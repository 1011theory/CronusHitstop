// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NiagaraComponent.h"
#include "Hitstop_Types.h"

#include "HitstopSubsystem.generated.h"

class UHitStopSubsystem;
class UHitStop;				
class UHitStopInterface;
class UHitStop_Cosmetic;

/* Grouped data for a single actor to hitstop*/
USTRUCT(BlueprintType) //@ToDo: Move to Hitstop_Types
struct CRONUSHITSTOP_API FHitStopTargetData 
{
	GENERATED_BODY()

	FHitStopTargetData() {}

	/* The actor to apply a hitstop to */
	UPROPERTY(BlueprintReadWrite, Category = "Hitstop")
	TObjectPtr<AActor> Actor = nullptr;

	/* VFX that the owner actor is responsible for and should be paused during the hitstop*/
	UPROPERTY(BlueprintReadWrite, Category = "Hitstop")
	TArray<TObjectPtr<UNiagaraComponent>> VFX;

	/* The mesh shake class - used to construct an actual mesh shake tied to the duration of the hitstop. */
	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "Hitstop")
	TArray<TSubclassOf<UHitStop_Cosmetic>> CosmeticEffects;

	UPROPERTY() // Faster during array checks in hitstop finish function
	TArray<TObjectPtr<UHitStop_Cosmetic>> ActiveCosmetics;

	/* If dealing with the instigator of a slice attack (sword hits, etc) we don't want to fully hitstop, but rather slow down the actor to an ~almost~ frozen state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Hitstop")
	bool bFullStop = true;

	// For lookups in arrays - we find the right entry by checking the actor.
	bool operator==(const FHitStopTargetData& Other) const
	{
		return Actor == Other.Actor;
	}

private:
	friend UHitStopSubsystem;

};


UCLASS()
class CRONUSHITSTOP_API UHitStopSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	/* Required to store and update references to the hitstops to avoid being garbage collected while a hitstop is still running */
	UPROPERTY()
	TArray<TObjectPtr<UHitStop>> ActiveHitStops;

	/* User error could potentially lead to multiple entries for the same actor when calling a hitstop. This function ensures one entry per actor and that they are all valid.*/
	TArray<FHitStopTargetData> RemoveDuplicates(const TArray<FHitStopTargetData>& Targets);

	bool CanActorBeHitStopped(AActor* Actor)const;


public:
	/* Simplifies the subsystem retrieval within the code base. */
	static UHitStopSubsystem& Get(const UWorld* World);

	/* Cleans up the target data before sending it to the hitstop object. */
	TArray<FHitStopTargetData> PrepareHitStopData(const TArray<FHitStopTargetData>& Targets, float Duration, const EHitstopDurationType Type);

	void RegisterHitStop(UHitStop* HitStop);
	void UnregisterHitStop(UHitStop* HitStop);

	/* Hitstops the specified targets. */
	UFUNCTION(BlueprintCallable, Category = "HitStop")
	void SimpleHitStop(const TArray<FHitStopTargetData>& Targets, const float Duration = 0.1f, const float Scale = 1.f, const EHitstopDurationType Type = EHitstopDurationType::TimeBased);

	void SimpleHitStop_Internal(const TArray<FHitStopTargetData>& Targets, const float Duration, const float Scale, const EHitstopDurationType Type, UHitStop*& OutHitStop);
	

	/* Cancels all hitstops registered with the subsystem. */
	UFUNCTION(BlueprintCallable, Category = "HitStop")
	void CancelAllActiveHitStops();
	
	//------------------------------------------------------------------------------------------------------------------
	// FTickableGameObject implementation Begin
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	// FTickableGameObject implementation End
	//------------------------------------------------------------------------------------------------------------------
private:
	UPROPERTY()
	uint64 LastTickedFrame;

};

