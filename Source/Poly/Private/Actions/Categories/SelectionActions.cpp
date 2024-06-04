// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshDecompositionFunctions.h"


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
				if (NumSelected > 2
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

					if (Positions.Num() > 2)
					{
						Center /= Positions.Num();
						FVector RefVector = (Center - Positions[0]).GetUnsafeNormal();
						FVector RefNormal = FVector::CrossProduct((Positions[0] - Center).GetUnsafeNormal(), (Positions[1] - Center).GetUnsafeNormal());
						//UE_LOG(LogTemp, Warning, TEXT("Normal: %s"), *RefNormal.ToString());
						
						Positions.Sort([Center, RefVector, RefNormal](const FVector& Pos0, const FVector& Pos1)
							{
								const float Dot = Pos0.X * Pos1.X + Pos0.Y * Pos1.Y + Pos0.Z * Pos1.Z;
								const float Det = FVector::DotProduct(RefNormal, FVector::CrossProduct(Pos0, Pos1));
								const float Angle = FMath::RadiansToDegrees(FMath::Atan2(Det, Dot));
								//UE_LOG(LogTemp, Warning, TEXT("\tAngle: %f"), Angle);
								return Angle < 0;
							});
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

bool UFlipPolygonAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;

	UDynamicMeshPool* Pool = NewObject<UDynamicMeshPool>(UDynamicMeshPool::StaticClass());
	UDynamicMesh* TempMesh = Pool->RequestMesh();

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
					UDynamicMesh* TargetMesh = Target->GetSelectedMesh();
					UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshSelectionToMesh(TargetMesh, TempMesh, Selection, TempMesh, false, true);
					int NumDeleted;
					UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteSelectedTrianglesFromMesh(TargetMesh, Selection, NumDeleted);
					UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(TempMesh);

					FGeometryScriptMeshSelection AllSelection;
					UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(TempMesh, AllSelection, EGeometryScriptMeshSelectionType::Triangles);
					UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshSelectionToMesh(TempMesh, TargetMesh, AllSelection, TargetMesh, true, true);
				}
			}
		}
		if (Selected.Num() > 0)
		{
			this->Submit();
			Pool->ReturnMesh(TempMesh);
			return true;
		}
	}
	this->Discard();
	Pool->ReturnMesh(TempMesh);
	return false;
}
