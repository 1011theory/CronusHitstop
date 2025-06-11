// Copyright The First Ones, 2024. All Rights Reserved.

#include "Cosmetics/HitStop_CosmeticContainer.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_CosmeticContainer)

UHitStop_CosmeticContainer::UHitStop_CosmeticContainer(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	UpdateType = ECosmeticDurationType::NoUpdate;
	bHideUpdateType = true;
}


void UHitStop_CosmeticContainer::StartEffect(AActor* ActorOwner)
{
	for (TSubclassOf<UHitStop_Cosmetic> CosmeticToAdd : Cosmetics)
	{
		if (CosmeticToAdd)
		{
			UHitStop_Cosmetic* NewCosmetic = NewObject<UHitStop_Cosmetic>(GetOuter(), CosmeticToAdd);
			if (ensure(NewCosmetic))
			{
				ActiveCosmetics.Add(NewCosmetic);
				NewCosmetic->SetInitialValues(StartTime, Duration, Scale);
				NewCosmetic->StartEffect(ActorOwner);
			}
		}
	}
	Super::StartEffect(ActorOwner);
}


void UHitStop_CosmeticContainer::EndEffect()
{
	Super::EndEffect();
	for (UHitStop_Cosmetic* Cosmetic : ActiveCosmetics)
	{
		if (Cosmetic)
		{
			Cosmetic->EndEffect();
		}
	}
	ActiveCosmetics.Empty();
	
}


void UHitStop_CosmeticContainer::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);
	if(bIsActive)
	{
		for (auto& Cosmetic : ActiveCosmetics)
		{
			if (Cosmetic)
			{
				Cosmetic->Update(DeltaSeconds);
			}
		}
	}
}


	

