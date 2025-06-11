// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Cosmetics/HitStop_Cosmetic_Blendable.h"

#include "HitStop_MeshCosmetic.generated.h"

/**
 * Base class for cosmetic effects that affect an actor's main mesh.
 */
UCLASS(NotBlueprintable, NotBlueprintType, Abstract)
class CRONUSHITSTOP_API UHitStop_MeshCosmetic : public UHitStop_Cosmetic_Blendable
{
	GENERATED_BODY()

	void SetMeshType();

	/* An optional name tag in case the target is not a character and has multiple meshes - the mesh cosmetic will attempt to find a tagged mesh */
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	bool bUseFallbackMesh;

	/* A name tag on the desired fallback mesh to shake on the target actor. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bUseFallbackMesh", EditConditionHides), Category = "Mesh")
	FName FallbackMeshTag;

	/* A name tag for edge-cases where we want to look for the fallback mesh even on character types*/
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bUseFallbackMesh", EditConditionHides), Category = "Mesh")
	FName CharacterFallbackMeshTag;
	
	
public:
	virtual void StartEffect(AActor* Actor) override;
	virtual void EndEffect() override;

	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> TargetMesh;
	
	
	
};
