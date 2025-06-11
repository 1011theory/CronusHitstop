// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/Mesh/HitStop_MeshCosmetic.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimInstance.h"
#include "HitStop_MeshAnimBlend.generated.h"

/**
 * Scrubs through the active montage on the target actor's mesh during a Hitstop.
 */
UCLASS(Blueprintable)
class CRONUSHITSTOP_API UHitStop_MeshAnimBlend : public UHitStop_MeshCosmetic
{
	GENERATED_BODY()

	UHitStop_MeshAnimBlend(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<UAnimMontage> AnimationMontage;

	/* Point in time to start animation from. */
	UPROPERTY(EditDefaultsOnly, Category = "Hitstop")
	float StartFrom = .5f;
	
	/* Point in time to resume animation from after Hitstop is complete. Normalized Between 0 and 1. */
	UPROPERTY(EditDefaultsOnly, Category = "Hitstop")
	float ResumeFrom = .5f;
	
	/* Allows designers to pick the montage playing in a specific slot in cases where multiple montages may be playing simultaneously. */
	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle), Category = "Hitstop")
	bool bUseMontageSlot = false;
	
	/* Allows designers to pick the montage playing in a specific slot in cases where multiple montages may be playing simultaneously. */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition="bUseMontageSlot"), Category = "Hitstop")
	FName SlotName = "DefaultSlot";

	
protected:
	virtual void StartEffect(AActor* ActorOwner) override;
	virtual void EndEffect() override;
	virtual void UpdateEffect_Internal() override;
	

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance; //weak it?

	/* Sorts through all active montages and returns the first montage that matches the requested Slot. */
	UAnimMontage* SortThroughMontages() const;
	
	//@ToDo: Interp Blend
	/* When the Montage is done blending in, the scrubbing can start */
	/*bool bBlendedIn = false;
	UPROPERTY(EditDefaultsOnly, Category = "Hitstop")
	float BlendTime = 0.f;*/
	//End Interp Blend

	float BlendTime = 0.f;
};

// @ToDo: How to manually interpolate between currently playing animation and this? -> workaround: lock movement, resume time dilation, and only apply time dilation after blend in time.
// @ToDo: Hide Intensity Curve Toggle.