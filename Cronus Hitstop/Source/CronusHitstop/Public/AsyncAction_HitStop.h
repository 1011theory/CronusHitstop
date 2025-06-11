// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "Hitstop_Types.h"
#include "AsyncAction_HitStop.generated.h"


class UHitStopSubsystem;
class UHitStop;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitStopEvent);
/**
 *
 */
UCLASS()
class CRONUSHITSTOP_API UAsyncAction_HitStop : public UCancellableAsyncAction
{
	GENERATED_BODY()

	/* A delegate called when the async action completes. */
	UPROPERTY(BlueprintAssignable)
	FHitStopEvent OnComplete;

	/* A delegate called when the async action fails. */
	UPROPERTY(BlueprintAssignable)
	FHitStopEvent OnCancelled;


public:
	UPROPERTY()
	TArray<FHitStopTargetData> ReceivedTargetData;

	float Duration = .1f;

	float Scale = 1.f;

	EHitstopDurationType Type = EHitstopDurationType::TimeBased;

	/* Creates a HitStop for the specified targets. On Complete fires if the hitstop went through successfully. On Cancelled fires if the hitstop was overriden by a higher priority hitstop. */
	UFUNCTION(BlueprintCallable, DisplayName = "Call HitStop and Wait", Category = "HitStop", meta = (WorldContext = "WorldContext", BlueprintInternalUseOnly = "true", Keywords = "Hitstop"))
	static UAsyncAction_HitStop* AsyncAction_HitStop(const UObject* WorldContext, const TArray<FHitStopTargetData> TargetData, const float Duration = 0.1f, const float Scale = 1.f, const EHitstopDurationType Type = EHitstopDurationType::TimeBased);

	// Override functions from parent
	virtual void Activate() override;
	virtual void Cancel() override;

	UFUNCTION()
	void HitStopCompleted();

	UFUNCTION()
	void HitStopCancelled();

	virtual UWorld* GetWorld() const override
	{
		return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
	}


private:
	/* The context world of this action. */
	TWeakObjectPtr<UWorld> ContextWorld;

	UPROPERTY()
	TObjectPtr<UHitStop> HitStopRef = nullptr;

};