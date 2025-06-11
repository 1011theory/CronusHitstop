// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "Sound/SoundMix.h"
#include "HitStop_FilterAudio.generated.h"


/**
 * Applies modifications through a soundmix during the hitstop. Mainly used to temporarily duck sound.
 */
UCLASS(Abstract)
class CRONUSHITSTOP_API UHitStop_FilterAudio : public UHitStop_Cosmetic
{
	GENERATED_BODY()

	UHitStop_FilterAudio(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	/* The sound mix used to duck sound during a hitstop. Soundmixes have settings for fade-ins, fade-outs, frequency and volume adjustments. Important to define which SoundClass a SoundMix affects to target the correct sounds (Background music, ambient sounds, etc) */
	UPROPERTY(EditDefaultsOnly, Category = "Hitstop")
	TObjectPtr<USoundMix> HitstopSoundMix; // Assets dont need explicit creation in code - (Factory and Asset Type actions) 

	
public:
	virtual void StartEffect(AActor* ActorOwner) override;
	virtual void EndEffect() override;

};
