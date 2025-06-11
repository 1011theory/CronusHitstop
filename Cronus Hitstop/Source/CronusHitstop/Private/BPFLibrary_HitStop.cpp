// Copyright The First Ones, 2024. All Rights Reserved.


#include "BPFLibrary_HitStop.h"
#include "HitStopSubsystem.h"
#include "Cosmetics/HitStop_Cosmetic.h"

FHitStopTargetData UBPFLibrary_HitStop::MakeSimpleHitStopData(AActor* TargetActor, UNiagaraComponent* VFX, UPARAM(meta = (AllowAbstract = false)) const TSubclassOf<UHitStop_Cosmetic> Cosmetic, const bool bFullStop)
{
	FHitStopTargetData NewTargetData;
	
	if (TargetActor)
	{
		NewTargetData.Actor = TargetActor;
	}
	if (VFX)
	{
		NewTargetData.VFX.Add(VFX);
	}
	if (Cosmetic)
	{
		NewTargetData.CosmeticEffects.Add(Cosmetic);
	}
	
	NewTargetData.bFullStop = bFullStop;
	
	return NewTargetData;
}
