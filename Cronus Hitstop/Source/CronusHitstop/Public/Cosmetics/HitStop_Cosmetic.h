// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "Curves/CurveFloat.h"
#include "HitStop_Cosmetic.generated.h"

class UWorld;

UENUM()
enum class ECosmeticDurationType : uint8 
{
	/* Time based cosmetics will update according to the set Frequency time interval. */
	TimeBased,
	/* Frame based cosmetics will update according to the set Frame Update Interval. */
	FrameBased,
	/* Cosmetics that don't need to update at any intervals. */
	NoUpdate
};


/**
 * A cosmetic effect to apply during a Hitstop.
 */
UCLASS(Blueprintable, NotBlueprintType, Abstract)
class CRONUSHITSTOP_API UHitStop_Cosmetic : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StartEffect(AActor* ActorOwner);

	virtual void EndEffect();

	virtual void Update(float DeltaSeconds);

	
protected:
	UHitStop_Cosmetic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PeriodicEffect();

	/* An internal version that lets the effect update, either through a timer or on the parent hitstop's tick update */
	virtual void UpdateEffect_Internal();

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bHideUpdateType", EditConditionHides), Category = "Hitstop")
	ECosmeticDurationType UpdateType = ECosmeticDurationType::TimeBased;

	/* Useful to prevent users for changing the update type in children classes only compatible with 1 update type */
	UPROPERTY()
	bool bHideUpdateType = false;

	/* Rate per second of the periodic cosmetic effect. Set it to 1 for simplicity if you want to control the update with the Frequency Curve. */
	UPROPERTY(EditDefaultsOnly, meta=(Units="s"), meta=(EditCondition="UpdateType == ECosmeticDurationType::TimeBased", EditConditionHides), Category = "Hitstop");
	float Frequency = 0.f; // if 0, timer will not be created.

	/* How many frames to wait each time the cosmetic is updated. Set it to 1 for simplicity if you want to control the update with the Frequency Curve. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="UpdateType == ECosmeticDurationType::FrameBased", EditConditionHides), Category = "Hitstop")
	int8 FrameUpdateInterval = 1;
	
	/* Instantly trigger the update/periodic function when the effect is applied, instead of waiting for the initial delay defined as Frequency. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="UpdateType == ECosmeticDurationType::TimeBased", EditConditionHides), Category = "Hitstop"); //@ToDo: make this work with frame based cosmetics
	bool bTriggerAtStart = true;

	/* Varies the frequency of the update of this effect over time. The duration of the effect is normalized between 0 and 1. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="UpdateType != ECosmeticDurationType::NoUpdate", EditConditionHides), Category = "Hitstop")
	FRuntimeFloatCurve UpdateCurve;
	
	/* Keeps track of how many frames have passed since the effect was started (if using DurationType::FrameBased). */
	int8 FrameCount = 0;

	/* For custom curves in children classes */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hitstop")
	float GetCurveValueAtCurrentTime(const FRuntimeFloatCurve& InCurve) const;
	
	virtual UWorld* GetWorld() const override;


	float GetTimePassed() const;
	float StartTime = 0.f;
	float Duration = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hitstop")
	float Scale = 1.f;

public:
	void SetInitialValues(const float InStartTime, const float InDuration, const float InScale) { StartTime = InStartTime; Duration = InDuration; Scale = InScale;}

	
private:
	FTimerHandle CosmeticTimer;


protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Hitstop")
	void OnEffectStarted(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Hitstop")
	void OnEffectEnded();

	UFUNCTION(BlueprintNativeEvent, Category = "Hitstop")
	void OnEffectUpdated();

public:
	bool bIsActive = false;

// Allows subclasses to run their initial setup before firing off the rest of the cosmetic logic.
	virtual void Initialize(){;}

};