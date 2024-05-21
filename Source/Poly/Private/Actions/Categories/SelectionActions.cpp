// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"


bool UDestroySelectedActorsAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolySelection*> SelectedActors = Selector->Selection;
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			UPolySelection* Selected = SelectedActors[i];
			bool IsSelected;
			Selector->Deselect(Selected, IsSelected);
			// destroy actor
			if (IsValid(Selected->GetSelectedActor()))
				Selected->GetSelectedActor()->Destroy();
		}
		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}

bool UDeleteSelectedElementsAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolySelection*> Selected = Selector->Selection;
		for (int i = 0; i < Selected.Num(); i++)
		{
			UPolyMeshSelection* Target = Cast<UPolyMeshSelection>(Selected[i]);
			if (IsValid(Target))
			{
				FGeometryScriptMeshSelection Selection = Target->GetMeshElementsSelection();
				if (Selection.GetNumSelected() > 0
					&& Selection.GetSelectionType() == EGeometryScriptMeshSelectionType::Triangles)
				{
					// ToDo: @tpott: check for polygroup selection type and convert selection temporarily to triangles and delete afterwards
					UDynamicMesh* TargetMesh = Target->GetSelectedMesh();
					int NumDeleted;
					UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteSelectedTrianglesFromMesh(TargetMesh, Selection, NumDeleted);

				}

			}
		}
		if (Selected.Num() > 0)
		{
			this->Submit();
			return true;
		}
	}
	this->Discard();
	return false;
}

bool UFillPolygonAction::Execute_Implementation(bool bEmitRecord)
{
	const FGeometryScriptPrimitiveOptions PrimitiveOptions = { EGeometryScriptPrimitivePolygroupMode::SingleGroup, false, EGeometryScriptPrimitiveUVMode::Uniform };

	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;

	TArray<FVector> Positions;
	TArray<int32> VertexIndices;

	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		TArray<UPolySelection*> Selected = Selector->Selection;
		for (int i = 0; i < Selected.Num(); i++)
		{
			UPolyMeshSelection* Target = Cast<UPolyMeshSelection>(Selected[i]);
			if (IsValid(Target))
			{
				FGeometryScriptMeshSelection Selection = Target->GetMeshElementsSelection();
				const int NumSelected = Selection.GetNumSelected();
				if (NumSelected > 0
					&& Selection.GetSelectionType() == EGeometryScriptMeshSelectionType::Vertices)
				{
					Positions.Reset(NumSelected);
					// ToDo: @tpott: check for polygroup selection type and convert selection temporarily to triangles and delete afterwards
					UDynamicMesh* TargetMesh = Target->GetSelectedMesh();
					Selection.ConvertToMeshIndexArray(TargetMesh->GetMeshRef(), VertexIndices, EGeometryScriptIndexType::Vertex);
					FVector Center = FVector::ZeroVector;
					for (int v = 0; v < NumSelected; v++)
					{
						bool bIsValid = false;
						FVector Position = UGeometryScriptLibrary_MeshQueryFunctions::GetVertexPosition(TargetMesh, VertexIndices[v], bIsValid);
						if (bIsValid)
						{
							Positions.Add(Position);
							Center += Position;
						}
					}

					//  ToDo: @tpott: Add sorting of positions (cw or ccw around center point)
					//				https://d3kjluh73b9h9o.cloudfront.net/original/4X/4/e/4/4e4192edeff20776d0716d61fa96aef0aa42edd3.png
					if (Positions.Num() > 0)
					{
						Center /= Positions.Num();
						FVector RefVector = (Center - Positions[0]).GetUnsafeNormal();
						Positions.Sort([Center, RefVector](const FVector& Pos0, const FVector& Pos1)
							{
								// access some random field just to test compile
								FVector Vec0 = (Center - Pos0).GetUnsafeNormal();
								FVector Vec1 = (Center - Pos1).GetUnsafeNormal();

								//  ToDo:@tpott: DotProduct gives direction of rotation from sign
								float Angle0 = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Vec0, RefVector)));
								float Angle1 = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Vec1, RefVector)));
								UE_LOG(LogTemp, Warning, TEXT("%s (%f)   -->   %s (%f)"), *Vec0.ToString(), Angle0, *Vec1.ToString(), Angle1)
									return  Angle0 < Angle1;
							});
						//for (int v = 0; v < Positions.Num(); v++)
						//{
						//	UE_LOG(LogTemp, Warning, TEXT("%d: %s"), v, *Positions[v].ToString())
						//}
						UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(TargetMesh, PrimitiveOptions, FTransform::Identity, Positions);
					}
				}

			}
		}
		if (Selected.Num() > 0)
		{
			this->Submit();
			return true;
		}
	}
	this->Discard();
	return false;
}
