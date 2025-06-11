// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/Mesh/HitStop_MeshAnimBlend.h"

#include "CronusHitstop.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_MeshAnimBlend)

UHitStop_MeshAnimBlend::UHitStop_MeshAnimBlend(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	bHideUpdateType = false;
	bShowIntensity = false;
	bUseIntensityCurve = false;
}


void UHitStop_MeshAnimBlend::StartEffect(AActor* ActorOwner)
{
	Super::StartEffect(ActorOwner);
	
	if (!TargetMesh)
	{
		return;
	}
	
	if (const USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(TargetMesh))
	{
		AnimInstance = SkeletalMesh->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}
		
		// If no montage is compatible, abort?
		if (bUseMontageSlot)
		{
			AnimationMontage = SortThroughMontages();
		}
		else
		{
			AnimationMontage = AnimInstance->GetCurrentActiveMontage();
		}

		if (AnimationMontage)
		{
			AnimationMontage->BlendIn= BlendTime;
					
			AnimInstance->Montage_Play(AnimationMontage);
					
			AnimInstance->Montage_SetPosition(AnimationMontage, StartFrom);
		}
		else
		{
			UE_LOG(LogCronus, Warning, TEXT("Attempted to animate mesh but no Animation Montage was found."));
		}
	}
}


void UHitStop_MeshAnimBlend::EndEffect()
{
	// Clear references here?
	if (AnimInstance && AnimationMontage)
	{
		const float PlayLength = AnimationMontage->GetPlayLength();
		if(ensureAlways(AnimInstance->Montage_IsPlaying(AnimationMontage)))
		{
			// this will be false if the montage is overriden by something else. Force it back in or let go? - Montage can also be cancelled if character (especially player) performs another action while hitstopped.
			AnimInstance->Montage_SetPosition(AnimationMontage, PlayLength * ResumeFrom); // Simply resume at post blend in?
		}
	}
	Super::EndEffect();
}


void UHitStop_MeshAnimBlend::UpdateEffect_Internal()
{
	Super::UpdateEffect_Internal();
	
	if (/*bBlendedIn && */AnimInstance && AnimationMontage)
	{
		AnimInstance->Montage_SetPosition(AnimationMontage, FMath::RandRange(0.f, 1.f)); // Scrubs through the animation frames
	}
}


UAnimMontage* UHitStop_MeshAnimBlend::SortThroughMontages() const
{
	// Start from end, as most recent instances are added at the end of the queue.
	int32 const NumInstances = AnimInstance->MontageInstances.Num();
	
	for (int32 InstanceIndex = NumInstances - 1; InstanceIndex >= 0; InstanceIndex--)
	{
		const FAnimMontageInstance* MontageInstance = AnimInstance->MontageInstances[InstanceIndex];
		if (MontageInstance && MontageInstance->IsActive() && MontageInstance->Montage->IsValidSlot(SlotName))
		{
			return MontageInstance->Montage;
		}
	}
	return nullptr;
}



