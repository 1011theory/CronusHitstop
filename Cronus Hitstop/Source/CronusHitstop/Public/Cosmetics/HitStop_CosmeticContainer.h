// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "HitStop_CosmeticContainer.generated.h"


/**
 * Gives the option to configure multiple cosmetics in a single container, to avoid adding them one by one when creating effects.
 */
UCLASS(Abstract)
class CRONUSHITSTOP_API UHitStop_CosmeticContainer : public UHitStop_Cosmetic
{
	GENERATED_BODY()

public:
	virtual void StartEffect(AActor* ActorOwner) override;
	virtual void EndEffect() override;
	virtual void Update(float DeltaSeconds) override;



	
protected:
	UHitStop_CosmeticContainer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, meta = (DisallowedClasses = "/Script/CronusHitstop.HitStop_CosmeticContainer"), Category = "Container")
	TArray<TSubclassOf<UHitStop_Cosmetic>> Cosmetics;

	UPROPERTY()
	TArray<TObjectPtr<UHitStop_Cosmetic>> ActiveCosmetics;
};
