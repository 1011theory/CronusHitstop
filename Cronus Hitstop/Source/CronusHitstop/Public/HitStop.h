// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "Hitstop_Types.h"
#include "Tickable.h"
#include "HitStop.generated.h"


class UWorld;
class UNiagaraComponent;
class UHitStopSubsystem;
class UHitStopInterface;
class UHitStop_Cosmetic;
struct FHitStopTargetData; 


/**
 * Stops all targets and their associated VFX for the defined duration, and applies all cosmetic effects found. Will be overridden by other hitstops with Duration > TimeLeft that affect any of the actors included in this one's target data.
 */
DECLARE_DELEGATE(FHitStopStatusSignature);

UCLASS()
class CRONUSHITSTOP_API UHitStop : public UObject/*, public FTickableGameObject*/
{
	GENERATED_BODY()


public:
	void Activate(const TArray<FHitStopTargetData>& InTargetData, const float InDuration, const float InScale, const EHitstopDurationType InDurationType  = EHitstopDurationType::TimeBased);
	
	void ChooseDurationType();
	
	FHitStopStatusSignature HitStopCompleted;

	FHitStopStatusSignature HitStopCancelled;

	/* instead of iterating through the structs, this could be used to check faster */
	bool AffectsActor(AActor* Actor) const;

	bool AffectsTargets(TArray<FHitStopTargetData> Targets) const;

	void CancelHitStop();
	
	float GetStartTime() const { return StartTime; }

	float GetDuration() const { return Duration; }

	EHitstopDurationType GetDurationType() const { return DurationType; }

	float GetTimePassed() const;

	float GetDurationSeconds() const;


private:
	/* The items to hit stop */
	UPROPERTY() 
	TArray<FHitStopTargetData> TargetData;

	float Duration = .1f;

	float Scale = 1.f;

	EHitstopDurationType DurationType;
	
	virtual UWorld* GetWorld() const override;

	void Cleanup();

	bool bIsActive = false;

	bool bActivationSuccessful = false;

public:
	bool IsActive() const {return bIsActive;}

private:
	/* Timer handle for Hitstop management */
	FTimerHandle OngoingDelay;

	void StartHitStop();

	void FinishHitStop();

	float StartTime;
	
	/* Used to count how many frames have passed during the hitstop */
	int32 TickCounter = 0;

	/* Notifies the cosmetics that a frame has passed. */
	void UpdateCosmetics(float DeltaSeconds);

public:
	void OnTick(float DeltaSeconds);
	
	bool bFinishedOnce = false;//debug
};
