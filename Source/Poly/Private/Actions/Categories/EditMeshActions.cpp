// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/EditMeshActions.h"

#include "EnumTypes.h"
#include "Selection/SelectorSubsystem.h"
#include "Modeling/PolyMeshSelection.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshDecompositionFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshTransformFunctions.h"


bool UDeleteElementsAction::Execute_Implementation(bool bEmitRecord)
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
				// ToDo: @tpott: add deletion of vertices from mesh, based on selection converted to index list,see: UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteVerticesFromMesh()

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

bool UCreatePolygonsAction::Execute_Implementation(bool bEmitRecord)
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

						Positions.Sort([Center, RefVector, RefNormal](const FVector& Pos0, const FVector& Pos1)
							{
								const float Dot = Pos0.X * Pos1.X + Pos0.Y * Pos1.Y + Pos0.Z * Pos1.Z;
								const float Det = FVector::DotProduct(RefNormal, FVector::CrossProduct(Pos0, Pos1));
								const float Angle = FMath::RadiansToDegrees(FMath::Atan2(Det, Dot));
								return Angle <= 0;
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

bool UFlipNormalsAction::Execute_Implementation(bool bEmitRecord)
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

bool UInsetOutsetFacesAction::Execute_Implementation(bool bEmitRecord)
{
	const FGeometryScriptMeshInsetOutsetFacesOptions Options = { Distance, bReproject, bBoundaryOnly, Softness, AreaScale,
		AreaMode, FGeometryScriptMeshEditPolygroupOptions(GroupMode, ConstantGroup), UVScale };

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
				UDynamicMesh* TargetMesh = Target->GetSelectedMesh();
				FGeometryScriptMeshSelection Selection = Target->GetMeshElementsSelection();
				if (Selection.GetNumSelected() > 0 && Selection.GetSelectionType() != EGeometryScriptMeshSelectionType::Vertices)
				{
					UGeometryScriptLibrary_MeshModelingFunctions::ApplyMeshInsetOutsetFaces(TargetMesh, Options, Selection);
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

bool UTessellateMeshActionBase::Execute_Implementation(bool bEmitRecord)
{
	switch (this->TessellateMeshMode)
	{
	case EPolyTessellateMeshActionMode::Uniform:
	{
		if (TessellateSelection_Uniform())
		{
			this->Submit();
			return true;
		}
		break;
	}
	case EPolyTessellateMeshActionMode::Concentric:
	{
		if (TessellateSelection_Concentric())
		{
			this->Submit();
			return true;
		}
		break;
	}
	}
	this->Discard();
	return false;
}

bool UTessellateMeshActionBase::TessellateSelection_Uniform()
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
				UDynamicMesh* TargetMesh = Target->GetSelectedMesh();

				// check if selection is empty, if empty selection behaviour is full mesh selection, create all mesh selection
				// continue with next otherwise (empty selection and behaviour is empty selection too).
				if (Selection.GetNumSelected() <= 0)
				{
					if (this->EmptySelectionBehavior == EGeometryScriptEmptySelectionBehavior::FullMeshSelection)
						UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(TargetMesh, Selection, EGeometryScriptMeshSelectionType::Polygroups);
					else continue;
				}

				UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshSelectionToMesh(TargetMesh, TempMesh, Selection, TempMesh, false, true);
				int NumDeleted;
				UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteSelectedTrianglesFromMesh(TargetMesh, Selection, NumDeleted);
				UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyUniformTessellation(TempMesh, TessellationLevel);

				FGeometryScriptMeshSelection AllSelection;
				UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(TempMesh, AllSelection, EGeometryScriptMeshSelectionType::Polygroups);
				UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshSelectionToMesh(TempMesh, TargetMesh, AllSelection, TargetMesh, true, true);
			}
		}
		if (Selected.Num() > 0)
		{
			Pool->ReturnMesh(TempMesh);
			return true;
		}
	}
	Pool->ReturnMesh(TempMesh);
	return false;
}

bool UTessellateMeshActionBase::TessellateSelection_Concentric()
{
	const FGeometryScriptSelectiveTessellateOptions Options = { true, EmptySelectionBehavior };

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
				UDynamicMesh* TargetMesh = Target->GetSelectedMesh();
				UGeometryScriptLibrary_MeshSubdivideFunctions::ApplySelectiveTessellation(TargetMesh, Selection, Options, TessellationLevel);
			}
		}
		if (Selected.Num() > 0)
		{
			return true;
		}
	}
	return false;
}


bool UTransformElementsSelectionAction::Execute_Implementation(bool bEmitRecord)
{
	USelectorSubsystem* SelectorSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USelectorSubsystem>();
	ASelectorBase* Selector;
	if (SelectorSubsystem->GetSelector(this, this->SelectorName, Selector))
	{
		const TArray<UPolySelection*> ActiveSelection = Selector->Selection;

		for (int i = 0; i < ActiveSelection.Num(); i++)
		{
			UPolyMeshSelection* Selection = Cast<UPolyMeshSelection>(ActiveSelection[i]);
			UDynamicMesh* TargetMesh = Selection->GetSelectedMesh();
			FGeometryScriptMeshSelection MeshSelection = Selection->GetMeshElementsSelection();

			UGeometryScriptLibrary_MeshTransformFunctions::TransformMeshSelection(TargetMesh, MeshSelection, this->DeltaTransform);
			//AActor* Selected = ActiveSelection[i]->GetSelectedActor();
			//Selected->AddActorWorldOffset(this->DeltaTransform.GetLocation());
			//Selected->AddActorWorldRotation(this->DeltaTransform.GetRotation());
			//Selected->SetActorScale3D(Selected->GetActorScale3D() + this->DeltaTransform.GetScale3D());
		}

		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}

inline void UTransformElementsSelectionAction::SetLocation(FVector InLocation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetLocation(InLocation);
}

inline void UTransformElementsSelectionAction::SetRotation(FQuat InRotation, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetRotation(InRotation);
}

inline void UTransformElementsSelectionAction::SetScale3D(FVector InScale, bool bClearTransform)
{
	if (bClearTransform)
		this->DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	this->DeltaTransform.SetScale3D(InScale);
}
