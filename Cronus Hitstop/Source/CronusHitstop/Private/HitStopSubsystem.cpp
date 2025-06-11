// Copyright The First Ones, 2024. All Rights Reserved.

#include "HitStopSubsystem.h"

#include "CronusHitstop.h"
#include "HitStopInterface.h"
#include "HitStop.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStopSubsystem)

UHitStopSubsystem& UHitStopSubsystem::Get(const UWorld* World) // simplifies retrieval.
{
	check(World);
	return *World->GetSubsystem<UHitStopSubsystem>(); //@ToDo: ???
}


bool UHitStopSubsystem::CanActorBeHitStopped(AActor* Actor) const // if hitstopped actor implements interface, check custom conditions.
{
	if (IsValid(Actor))
	{
		if(Actor->GetClass()->ImplementsInterface(UHitStopInterface::StaticClass()))
		{
			return IHitStopInterface::Execute_CanBeHitStopped(Actor);
		}
		return true;
	}
	return false;
}


void UHitStopSubsystem::SimpleHitStop(const TArray<FHitStopTargetData>& Targets, const float Duration, const float Scale, const EHitstopDurationType Type)
{
	UHitStop* TempHitStop;
	SimpleHitStop_Internal(Targets, Duration, Scale, Type, TempHitStop);
}


void UHitStopSubsystem::SimpleHitStop_Internal(const TArray<FHitStopTargetData>& Targets, const float Duration, const float Scale, const EHitstopDurationType Type, UHitStop*& OutHitStop)
{
	OutHitStop = nullptr;
	
	const TArray<FHitStopTargetData> FinalTargets = PrepareHitStopData(Targets, Duration, Type);

	if (FinalTargets.IsEmpty()) // if no targets can be hitstopped.
	{
		UE_LOG(LogCronus, Warning, TEXT("Attempted to prepare hitstops but final list was empty"));
		return;
	}

	UHitStop* HitStop = NewObject<UHitStop>(this);
	if (ensure(HitStop))
	{
		RegisterHitStop(HitStop);
		HitStop->Activate(FinalTargets, Duration, Scale, Type);
	}

	OutHitStop = HitStop;
}


void UHitStopSubsystem::CancelAllActiveHitStops()
{
	for (int32 Index = ActiveHitStops.Num() - 1; Index >= 0; --Index)
	{
		if (UHitStop* HitStop = ActiveHitStops[Index])
		{
			HitStop->CancelHitStop();
			// @ToDo: currently removing from array from the hitstop object. Remove here?
		}	
	}
}


TStatId UHitStopSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UHitStopSubsystem, STATGROUP_Tickables);
}


void UHitStopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LastTickedFrame = GFrameCounter;
}

void UHitStopSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


void UHitStopSubsystem::Tick(float DeltaTime)
{
	if (LastTickedFrame < GFrameCounter)
	{
		LastTickedFrame = GFrameCounter;

		if (GetWorld() != nullptr)
		{
			for (int32 i = ActiveHitStops.Num() - 1; i >= 0; i--)
			{
				UHitStop* HitStop = ActiveHitStops[i];
		
				if (ensureAlways(IsValid(HitStop)))
				{
					if(HitStop->IsActive())
					{
						HitStop->OnTick(DeltaTime);
					}
				}
			}
		}
	}
}


TArray<FHitStopTargetData> UHitStopSubsystem::PrepareHitStopData(const TArray<FHitStopTargetData>& Targets, float Duration, const EHitstopDurationType Type)
{
	TArray<FHitStopTargetData> UniqueTargets = RemoveDuplicates(Targets);
	TArray<FHitStopTargetData> FinalTargetsToHitStop;
	
	if (Type == EHitstopDurationType::FrameBased) //converts to real world time for comparisons if using frame based hitstops
	{
		Duration = Duration * GetWorld()->GetDeltaSeconds();
	}
	
	// the system will check if any of the active hitstops affect the new target actors, 
	// and if so whether they have more time left than the new one - we dont want short hitstops to override more important ones.
	for (const FHitStopTargetData& NewTarget : UniqueTargets)
	{
		bool bFoundMatch = false;
		bool bHasHigherPriorityHitStop = false;

		for (int32 i = ActiveHitStops.Num() - 1; i >= 0; i--)
		{
			UHitStop* HitStop = ActiveHitStops[i];

			if (IsValid(HitStop))
			{
				if(HitStop->IsActive())
				{
					if (HitStop->AffectsActor(NewTarget.Actor))
					{
						bFoundMatch = true;
						// here we want to check if the stop is time/frame based and use that to decide how to calculate time passed
						const float CurrentDuration = GetWorld()->GetTimeSeconds() - HitStop->GetStartTime();
						float TimeLeft = HitStop->GetDuration() - CurrentDuration;
					
						if (HitStop->GetDurationType() == EHitstopDurationType::FrameBased) // add additional check to see if its just "different"
						{
							// adjust duration to be time based if new hitstop is not frame based
							// frames divided by how many frames in a second =
							const float ConvertedFrameTimeToSeconds = HitStop->GetDuration() * GetWorld()->GetDeltaSeconds();
							TimeLeft = ConvertedFrameTimeToSeconds - CurrentDuration;
						}

						if (Duration > TimeLeft && CanActorBeHitStopped(NewTarget.Actor))
						{
							bFoundMatch = true;
							
							if(ensure(IsValid(NewTarget.Actor)))//cancellation could lead to its destruction
							{
								UE_LOG(LogCronus, Log, TEXT("Cancelled ongoing hitstop for : %s"), *GetNameSafe(NewTarget.Actor));
								HitStop->CancelHitStop(); // this will remove hitstop from the list
								FinalTargetsToHitStop.Add(NewTarget);
							}
						}
						else
						{
							// if there's a match, but the currently active hitstop will last longer than the new one we are trying to add
							bHasHigherPriorityHitStop = true;
						}
						break;
					}	
				}
			}
		}
		if (!bFoundMatch && !bHasHigherPriorityHitStop)
		{
			if (CanActorBeHitStopped(NewTarget.Actor))
			{
				FinalTargetsToHitStop.Add(NewTarget);
			}
		}
	}
	return FinalTargetsToHitStop;
}


void UHitStopSubsystem::RegisterHitStop(UHitStop* HitStop)
{
	if (ensureAlways(!ActiveHitStops.Contains(HitStop)))
	{
		ActiveHitStops.Add(HitStop);
	}
}


void UHitStopSubsystem::UnregisterHitStop(UHitStop* HitStop)
{
	if (ensureAlways(ActiveHitStops.Contains(HitStop)))
	{
		ActiveHitStops.Remove(HitStop);
	}
}


TArray<FHitStopTargetData> UHitStopSubsystem::RemoveDuplicates(const TArray<FHitStopTargetData>& Targets)
{
	// Use a set instead of array to avoid unnecessary iterations
	TSet<AActor*> UniqueActors;

	// Use this array to store final unique targets
	TArray<FHitStopTargetData> UniqueTargets;

	// Reserve memory for the unique targets array to reduce reallocation costs - since its unlikely to change length.
	UniqueTargets.Reserve(Targets.Num());

	for (const FHitStopTargetData& Item : Targets)
	{
		if (ensure(IsValid(Item.Actor)))
		{
			if (!UniqueActors.Contains(Item.Actor))
			{
				UniqueActors.Add(Item.Actor);

				UniqueTargets.Add(Item);
			}
			else
			{
				UE_LOG(LogCronus, Warning, TEXT("Sent multiple entries of same actor %s to the Hitstop System!"), *GetNameSafe(Item.Actor))
			}
		}
	}
	return UniqueTargets;
}








