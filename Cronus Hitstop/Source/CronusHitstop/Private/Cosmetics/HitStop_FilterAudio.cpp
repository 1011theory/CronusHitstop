// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/HitStop_FilterAudio.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_FilterAudio)

UHitStop_FilterAudio::UHitStop_FilterAudio(const FObjectInitializer& ObjectInitializer)
{
	UpdateType = ECosmeticDurationType::NoUpdate;
	bHideUpdateType = true;
}


void UHitStop_FilterAudio::StartEffect(AActor* ActorOwner)
{
	if (HitstopSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, HitstopSoundMix); 
	}
	Super::StartEffect(ActorOwner);
}


void UHitStop_FilterAudio::EndEffect()
{
	if (HitstopSoundMix)
	{
		UGameplayStatics::PopSoundMixModifier(this, HitstopSoundMix);
	}
	HitstopSoundMix = nullptr;
	Super::EndEffect();
}