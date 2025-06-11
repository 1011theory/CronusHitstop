// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitStopSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BPFLibrary_HitStop.generated.h"

class UHitStopSubsystem;
class UHitStop;
class UHitStop_Cosmetic;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class CRONUSHITSTOP_API UBPFLibrary_HitStop : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* Helper node to make simple hitstop data (one entry per type) without introducing the clutter of "makearray" nodes for VFX and Cosmetics entries. */
	UFUNCTION(BlueprintPure, Category = "HitStop", meta = (AdvancedDisplay = "Cosmetic, bFullStop"))
	static FHitStopTargetData MakeSimpleHitStopData(AActor* TargetActor, UNiagaraComponent* VFX, UPARAM(meta = (AllowAbstract = false)) const TSubclassOf<UHitStop_Cosmetic> Cosmetic, const bool bFullStop = true);
	
};
