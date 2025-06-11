// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/HitStop_Cosmetic_Blendable.h"
#include "Camera/CameraShakeBase.h"
#include "HitStop_CameraShake.generated.h"


class APlayerCameraManager;

/**
 * Starts and stops a camera shake during the lifetime of a hitstop.
 */
UCLASS(Blueprintable, Abstract)
class CRONUSHITSTOP_API UHitStop_CameraShake : public UHitStop_Cosmetic_Blendable
{
	GENERATED_BODY()

	UHitStop_CameraShake(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	float ShakeScale = 1.f;

	UPROPERTY()
	TObjectPtr<UCameraShakeBase> ActiveShake;

	
public:
	virtual void StartEffect(AActor* ActorOwner) override;
	virtual void EndEffect() override;
	virtual void UpdateEffect_Internal() override;
	
};
