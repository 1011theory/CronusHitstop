// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/HitStop_CameraShake.h"

#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_CameraShake)

UHitStop_CameraShake::UHitStop_CameraShake(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	UpdateType = ECosmeticDurationType::FrameBased;
	bHideUpdateType = true;
}


void UHitStop_CameraShake::StartEffect(AActor* ActorOwner)
{
	if (ensure(CameraShake))
	{
		if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
		{
			ActiveShake = CamManager->StartCameraShake(CameraShake, ShakeScale); //@ToDo: should be set to scaled amount already ?
		}
	}
	Super::StartEffect(ActorOwner);
}


void UHitStop_CameraShake::EndEffect()
{
	if (ActiveShake)
	{
		if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
		{
			CamManager->StopCameraShake(ActiveShake);
			ActiveShake = nullptr;
		}
	}
	Super::EndEffect();
}


void UHitStop_CameraShake::UpdateEffect_Internal()
{
	Super::UpdateEffect_Internal();
	if (ActiveShake && bUseIntensityCurve)
	{
		ActiveShake->ShakeScale = ShakeScale * GetIntensityAtCurrentTime() * Scale;
	}
}
