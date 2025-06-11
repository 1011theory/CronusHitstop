// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/HitStop_Cosmetic.h"

#include "HitStopSubsystem.h"
#include "Engine/World.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_Cosmetic)

UHitStop_Cosmetic::UHitStop_Cosmetic(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	UpdateCurve.EditorCurveData.AddKey(0, 1);
	UpdateCurve.EditorCurveData.AddKey(1, 1);
}


void UHitStop_Cosmetic::StartEffect(AActor* ActorOwner)
{
	if (GetWorld() && (ensure(IsValid(ActorOwner))))
	{
		Initialize();
		bIsActive = true;
		switch (UpdateType)
		{
		case ECosmeticDurationType::FrameBased:
			break;
			
		case ECosmeticDurationType::TimeBased:
			{
				if (bTriggerAtStart)
				{
					UpdateEffect_Internal();
				}
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &UHitStop_Cosmetic::PeriodicEffect);
				const float AdjustedFrequency = Frequency * GetCurveValueAtCurrentTime(UpdateCurve);
				GetWorld()->GetTimerManager().SetTimer(CosmeticTimer, Delegate, FMath::Max(AdjustedFrequency, 0.01f), false);
			}
			break;
			
		case ECosmeticDurationType::NoUpdate:
			break;
		}
		OnEffectStarted(ActorOwner);
	}
}


void UHitStop_Cosmetic::EndEffect()
{
	bIsActive = false;
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CosmeticTimer);
	}
	OnEffectEnded();
}


void UHitStop_Cosmetic::Update(float DeltaSeconds)
{
	if(!bIsActive)
	{
		return;
	}
		if (UpdateType == ECosmeticDurationType::FrameBased)
		{
			const int8 AdjustedFrameUpdateInterval = FrameUpdateInterval * GetCurveValueAtCurrentTime(UpdateCurve);
			if (FrameCount % FMath::Max(AdjustedFrameUpdateInterval, 1) == 0)
			{
				UpdateEffect_Internal();
			}
			FrameCount++;
		}
}


void UHitStop_Cosmetic::PeriodicEffect()
{
	if (const UWorld* World = GetWorld())
	{
		if(ensure(IsValid(this)))
		{
			if(!bIsActive)
			{
				return;
			}
			UpdateEffect_Internal();
		
			const float AdjustedFrequency = Frequency * GetCurveValueAtCurrentTime(UpdateCurve);
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UHitStop_Cosmetic::PeriodicEffect);
			World->GetTimerManager().SetTimer(CosmeticTimer, Delegate, AdjustedFrequency, false);
		}
		
	}
}


void UHitStop_Cosmetic::UpdateEffect_Internal()
{
	OnEffectUpdated();
}


float UHitStop_Cosmetic::GetCurveValueAtCurrentTime(const FRuntimeFloatCurve& InCurve) const
{
	const float Alpha = GetTimePassed() / FMath::Max(Duration, KINDA_SMALL_NUMBER); // To avoid division by 0;
	return InCurve.GetRichCurveConst()->Eval(Alpha);
}


UWorld* UHitStop_Cosmetic::GetWorld() const
{
	if (const UHitStopSubsystem* Subsystem = Cast<UHitStopSubsystem>(GetOuter()))
	{
		return Subsystem->GetWorld();
	}
	return nullptr;
}


float UHitStop_Cosmetic::GetTimePassed() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetTimeSeconds() - StartTime;
	}
	return 0;
}


void UHitStop_Cosmetic::OnEffectStarted_Implementation(AActor* TargetActor)
{}


void UHitStop_Cosmetic::OnEffectEnded_Implementation()
{}


void UHitStop_Cosmetic::OnEffectUpdated_Implementation()
{}

