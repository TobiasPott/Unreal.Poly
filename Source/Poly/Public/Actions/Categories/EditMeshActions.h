// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "Selection/SelectorTypes.h"
#include "Gizmos/GizmoTypes.h"
#include "EditMeshActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UDeleteElementsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UDeleteElementsAction() : UActionBase("poly.DeleteElements", "Delete Elements") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UCreatePolygonsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UCreatePolygonsAction() : UActionBase("poly.CreatePolygons", "Create Polygons") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};



UCLASS(Blueprintable)
class POLY_API UFlipNormalsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UFlipNormalsAction() : UActionBase("poly.FlipNormals", "Flip FlipNormals") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


UCLASS(Blueprintable)
class POLY_API UInsetOutsetFacesAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UInsetOutsetFacesAction() : UActionBase("poly.InsetOutsetFaces", "Inset Outset Faces") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

	// ToDo: @tpott: add further members which should be exposed from the FGeometryScriptMeshInsetOutsetFacesOptions struct (e.g. distance etc.)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	float Distance = 10.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	bool bReproject = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	bool bBoundaryOnly = false; // ToDo: @tpott: Don't know what this member actually affects in the InsetOutsetFaces function
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	float Softness = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	float AreaScale = 1.0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	EGeometryScriptPolyOperationArea AreaMode = EGeometryScriptPolyOperationArea::EntireSelection;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	EGeometryScriptMeshEditPolygroupMode GroupMode = EGeometryScriptMeshEditPolygroupMode::PreserveExisting;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	int ConstantGroup = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Default, meta = (ExposeOnSpawn = "true"))
	float UVScale = 1.0;

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};


/**
 * Behavior of tessellate mesh actions when applied to a mesh or mesh selection
 */
UENUM(BlueprintType)
enum class EPolyTessellateMeshActionMode : uint8
{
	Uniform = 0,		// uses the MeshSubdivideFunctions::ApplyUniformTessellation
	Concentric = 1,		// uses the MeshSubdivideFunctions::ApplySelectiveTessellation
};

UCLASS(Blueprintable)
class POLY_API UTessellateMeshActionBase : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UTessellateMeshActionBase() : UActionBase("poly.UTessellateMeshActionBase", "<UTessellateMeshActionBase>") {}
	UTessellateMeshActionBase(const FString& InDescription, const FString& InShortName) : UActionBase(InDescription, InShortName) { }


	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true", ClampMin = 1, ClampMax = 8))
	EPolyTessellateMeshActionMode TessellateMeshMode = EPolyTessellateMeshActionMode::Uniform;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true", ClampMin = 1, ClampMax = 8))
	int TessellationLevel = 1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	EGeometryScriptEmptySelectionBehavior EmptySelectionBehavior = EGeometryScriptEmptySelectionBehavior::FullMeshSelection;

public:
	bool Execute_Implementation(bool bEmitRecord) override;

protected:
	bool TessellateSelection_Uniform();
	bool TessellateSelection_Concentric();
};



UCLASS(Blueprintable)
class POLY_API UTessellateMeshConcentricAction : public UTessellateMeshActionBase
{
	GENERATED_BODY()
public:
	// Ctor
	UTessellateMeshConcentricAction() : UTessellateMeshActionBase("poly.TessellateMeshConcentric", "Tessellate Mesh (Concentric)") { this->TessellateMeshMode = EPolyTessellateMeshActionMode::Concentric; }
	// Members
};

UCLASS(Blueprintable)
class POLY_API UTessellateMeshUniformAction : public UTessellateMeshActionBase
{
	GENERATED_BODY()
public:
	// Ctor
	UTessellateMeshUniformAction() : UTessellateMeshActionBase("poly.TessellateMeshUniform", "Tessellate Mesh (Uniform)") { this->TessellateMeshMode = EPolyTessellateMeshActionMode::Uniform; }
	// Members
};


UCLASS(Blueprintable)
class POLY_API UTransformElementsSelectionAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UTransformElementsSelectionAction() : UActionBase("poly.TransformElementsSelection", "Transform Elements Selection") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Actors;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	ETransformSpace Space = ETransformSpace::TS_World;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FTransform DeltaTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);

public:
	bool Execute_Implementation(bool bEmitRecord) override;

	void SetupWith(FName InSelectorName, ETransformSpace InSpace, FTransform InDeltaTransform)
	{
		this->SelectorName = InSelectorName;
		this->Space = InSpace;
		this->DeltaTransform = InDeltaTransform;
	}
	void SetLocation(FVector InLocation, bool bClearTransform = false);
	void SetRotation(FQuat InRotation, bool bClearTransform = false);
	void SetRotation(FRotator InRotation, bool bClearTransform = false) { SetRotation(InRotation.Quaternion(), bClearTransform); }
	void SetScale3D(FVector InScale, bool bClearTransform = false);
};
