// Copyright The First Ones, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "Curves/CurveFloat.h"
#include "HitStop_Cosmetic_Blendable.generated.h"

/**
 * Base class for cosmetics that can have their blend intensity changed over time.
 */
UCLASS(NotBlueprintable, NotBlueprintType, Abstract)
class CRONUSHITSTOP_API UHitStop_Cosmetic_Blendable : public UHitStop_Cosmetic
{
	GENERATED_BODY()

protected:
	UHitStop_Cosmetic_Blendable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly,Category = "Hitstop")
	bool bShowIntensity = true;
	
	/* Enable this if the cosmetic can have its intensity value changed over time. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "bShowIntensity", EditConditionHides), Category = "Hitstop")
	bool bUseIntensityCurve = true;
	
	/* Multiplier curve used to change the intensity of various cosmetics during the lifetime of a Hitstop. Hitstop Duration is normalized between 0 and 1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "bUseIntensityCurve", EditConditionHides), Category = "Hitstop")
	FRuntimeFloatCurve IntensityCurve;
	
	/* This returns the multiplier value defined by the Intensity curve, according to how far through a Hitstop we are. */
	UFUNCTION(BlueprintCallable, Category = "Hitstop")
	float GetIntensityAtCurrentTime() const;
	
};
