// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/Mesh/HitStop_MeshCosmetic.h"
#include "Materials/MaterialInterface.h"

#include "HitStop_MaterialAnimation.generated.h"


/**
 * Assigns an overlay material to the target for the duration of the hitstop, and removes it (or replaces it with previously active overlay materials)
 * when the hitstop is complete. Makes a float custom primitive index value on the material switch between 0 and 1, at the defined Frequency rate.
 */
UCLASS(Blueprintable, Abstract)
class CRONUSHITSTOP_API UHitStop_MaterialAnimation : public UHitStop_MeshCosmetic
{
	GENERATED_BODY()
	
	/* The overlay Material applied during the hitstop. If the target already had an overlay material before, it will be reapplied after this effect expires. */
	UPROPERTY(EditDefaultsOnly, Category = "Material Animation")
	TObjectPtr<UMaterialInterface> OverlayMaterial;

	/* The Custom Primitive Data Index value affected by this. Set it to a number that isnt commonly used by other materials, since it's value will be modified indefinitely. */
	UPROPERTY(EditDefaultsOnly, Category = "Material Animation")
	int32 MaterialDataIndex = 0;
	
	virtual void Initialize() override;


public:
	virtual void StartEffect(AActor* Actor) override;
	virtual void EndEffect() override;
	virtual void UpdateEffect_Internal() override;

	
private:
	bool bFlipFlop = false;

	/* The overlay material that the character had before the hitstop, or nullptr if none*/
	UPROPERTY()
	TObjectPtr<UMaterialInterface> OldOverlayMaterial = nullptr;
	
};