// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshBasicEditFunctions.h"


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
