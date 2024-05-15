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
		TArray<AActor*> SelectedActors = Selector->Selection;
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			AActor* Selected = SelectedActors[i];
			bool IsSelected;
			Selector->Deselect(Selected, IsSelected);
			Selected->Destroy();
		}
		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}

bool UDeleteSelectedElementsAction::Execute_Implementation(bool bEmitRecord)
{
	if (this->Selection.GetNumSelected() > 0
		&& this->Selection.GetSelectionType() == EGeometryScriptMeshSelectionType::Triangles)
	{
		UDynamicMesh* TargetMesh = Target->GetComponentByClass<UBaseDynamicMeshComponent>()->GetDynamicMesh();
		int NumDeleted;
		UGeometryScriptLibrary_MeshBasicEditFunctions::DeleteSelectedTrianglesFromMesh(TargetMesh, this->Selection, NumDeleted);

		this->Submit();
		return true;
	}

	this->Discard();
	return false;
}
