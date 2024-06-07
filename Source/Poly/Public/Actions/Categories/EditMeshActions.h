// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actions/ActionBase.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "Selection/SelectorTypes.h"
#include "EditMeshActions.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class POLY_API UDeleteMeshElementsAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	UDeleteMeshElementsAction() : UActionBase("poly.DeleteMeshElements", "Delete Mesh Elements") {}

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
class POLY_API USubdivideMeshAction : public UActionBase
{
	GENERATED_BODY()

public:
	// Ctor
	USubdivideMeshAction() : UActionBase("poly.SubdivideMesh", "Subdivide Mesh") {}

	// Members
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FName SelectorName = USelectorNames::Elements;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true", ClampMin = 1, ClampMax = 8)) // limit max tesellation level to 8 on spawn
	int TessellationLevel = 1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	ESelectiveTessellatePatternType PatternType = ESelectiveTessellatePatternType::ConcentricRings;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
	EGeometryScriptEmptySelectionBehavior EmptySelectionBehavior = EGeometryScriptEmptySelectionBehavior::FullMeshSelection;

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

public:
	bool Execute_Implementation(bool bEmitRecord) override;
};