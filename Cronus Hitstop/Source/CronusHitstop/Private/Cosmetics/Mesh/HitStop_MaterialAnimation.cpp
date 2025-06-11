// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/Mesh/HitStop_MaterialAnimation.h"

#include "CronusHitstop.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_MaterialAnimation)

void UHitStop_MaterialAnimation::Initialize()
{
	Super::Initialize();
	if(TargetMesh)
	{
		check(OldOverlayMaterial.GetClass() != OverlayMaterial.GetClass());
		OldOverlayMaterial = TargetMesh->GetOverlayMaterial();
		TargetMesh->SetOverlayMaterial(OverlayMaterial);
	}
}


void UHitStop_MaterialAnimation::StartEffect(AActor* Actor)
{
	
	Super::StartEffect(Actor);
}


void UHitStop_MaterialAnimation::EndEffect()
{
	if (TargetMesh)
	{
		if(TargetMesh->GetOverlayMaterial() == OverlayMaterial) // it's possible that another system changes the overlay during this cosmetic. in that case, we dont want to remove the overlay.
		{
			TargetMesh->SetOverlayMaterial(OldOverlayMaterial);
			if(OldOverlayMaterial)
			{
				UE_LOG(LogCronus, Log, TEXT("Reapplied old overlay material %s"), *GetNameSafe(OldOverlayMaterial))
			}
		}
		
	}
	Super::EndEffect();
}


void UHitStop_MaterialAnimation::UpdateEffect_Internal()
{
	Super::UpdateEffect_Internal();

	if (TargetMesh && OverlayMaterial)
	{
		TargetMesh->SetCustomPrimitiveDataFloat(MaterialDataIndex, bFlipFlop ? 0 : GetIntensityAtCurrentTime() * Scale);
	}
	bFlipFlop = !bFlipFlop;
}



