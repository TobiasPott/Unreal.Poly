// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Categories/SelectionActions.h"
#include "Selection/SelectorSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "GeometryScript/MeshBasicEditFunctions.h"


bool UDestroySelectedAction::Execute_Implementation(bool bEmitRecord)
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
	if (IsValid(this->Target))
	{
		FGeometryScriptMeshSelection Selection = this->Target->GetMeshElementsSelection();
		if (Selection.GetNumSelected() > 0
			&& Selection.GetSelectionType() == EGeometryScriptMeshSelectionType::Triangles)
		{
			// ToDo: @tpott: check for polygroup selection type and convert selection temporarily to triangles and delete afterwards
			UDynamicMesh* TargetMesh = this->Target->GetSelectedMesh();
			int NumDeleted;
			UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteSelectedTrianglesFromMesh(TargetMesh, Selection, NumDeleted);

			this->Submit();
			return true;
		}

	}
	this->Discard();
	return false;
}
