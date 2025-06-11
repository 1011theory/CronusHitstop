// Copyright The First Ones, 2024. All Rights Reserved.

#include "HitStop.h"

#include "CronusHitstop.h"
#include "HitStopSubsystem.h"
#include "HitStopInterface.h"
#include "NiagaraComponent.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop)

// After cleaning the target data to ensure no duplicates, no nullptr, and no durations < than new duration, the subsystem sends the data here
void UHitStop::Activate(const TArray<FHitStopTargetData>& InTargetData, const float InDuration, const float InScale, const EHitstopDurationType InDurationType)
{
    if (GetWorld())
    {
        TargetData = InTargetData;
        Duration = InDuration;
        DurationType = InDurationType;
        Scale = InScale;

        check(!TargetData.IsEmpty());
        if (TargetData.IsEmpty())
        {
            HitStopCompleted.ExecuteIfBound(); 
            Cleanup();
            bActivationSuccessful = true;
            return;
        }

        StartHitStop();
        
        return;
    }
    Cleanup();// If hit stop fails to start
}


void UHitStop::ChooseDurationType() // we could store the duration type so that it can be refd by the subsystem when comparing durations
{
    switch (DurationType) //@ToDo: move inners to their own functions and keep switch statement simpler
    {
    case EHitstopDurationType::TimeBased:
        {
            FTimerManager& TimerManager = GetWorld()->GetTimerManager();
            TimerManager.SetTimer(OngoingDelay, FTimerDelegate::CreateLambda([WeakThis = TWeakObjectPtr<UHitStop>(this)]()
                {
                    if (WeakThis.IsValid() && WeakThis->bIsActive)
                    {
                        WeakThis->HitStopCompleted.ExecuteIfBound();
                        WeakThis->Cleanup(); // In 5.5.3, garbage collection kicks in right after the subsystem removes the hitstop from the array, which caused crashes to one user.
                        
                    }
                }),
                FMath::Max(Duration, 0.001f), false);
            break;
        }
    case EHitstopDurationType::FrameBased:
        const int32 IntDuration = Duration; // converts duration to integer in case user inputs decimals
        Duration = FMath::Max(IntDuration, 1); //ensure the hitstop will last at least 1 frame
        break;
    }
    bIsActive = true;
}


void UHitStop::StartHitStop()
{
    StartTime = GetWorld()->GetTimeSeconds();
    check (!TargetData.IsEmpty());
    for (FHitStopTargetData& SingleTargetData : TargetData)
    {
        if (ensure(IsValid(SingleTargetData.Actor)))
        {
            SingleTargetData.Actor->CustomTimeDilation = SingleTargetData.bFullStop ? 0.f : 0.01f; //@ToDo make an advanced display variable that sets the default non-fullstop dilation.

            for (auto& VFX : SingleTargetData.VFX)
            {
                if (VFX)
                {
                    VFX->SetForceSolo(true);
                    VFX->SetCustomTimeDilation(0.0001f); //@ToDo: setup with 0.faa
                }
            }
            for (auto& CosmeticEffect : SingleTargetData.CosmeticEffects)
            {
                if (CosmeticEffect)
                {
                    UHitStopSubsystem& Subsystem = UHitStopSubsystem::Get(GetWorld());
                    UHitStop_Cosmetic* NewCosmetic = NewObject<UHitStop_Cosmetic>(&Subsystem, CosmeticEffect);
                    if (ensure(NewCosmetic))
                    {
                        SingleTargetData.ActiveCosmetics.Add(NewCosmetic);
                        NewCosmetic->SetInitialValues(StartTime, GetDurationSeconds(), Scale);
                        NewCosmetic->StartEffect(SingleTargetData.Actor);
                    }
                }
            }    
        }
        if (SingleTargetData.Actor && SingleTargetData.Actor->GetClass()->ImplementsInterface(UHitStopInterface::StaticClass()))
        {
            IHitStopInterface::Execute_OnHitStopStarted(SingleTargetData.Actor);
        }
       
        UE_LOG(LogCronus, Log, TEXT("Added hitstop for actor : %s"), *GetNameSafe(SingleTargetData.Actor));
    }
    ChooseDurationType();
}


void UHitStop::Cleanup()
{
    if (!GetWorld())
    {
        return;
    }
    
    bIsActive = false; //was after finish hitstop

    if (const UWorld* World = GetWorld())
    {
        FTimerManager& TimerManager = World->GetTimerManager();
        TimerManager.ClearTimer(OngoingDelay);
    }
    
    FinishHitStop();
}


void UHitStop::FinishHitStop()
{
    if(bFinishedOnce)
    {
        UE_LOG(LogCronus, Warning, TEXT("Attempted to call FinishHitstop while it had already been finished."));
        return;
    }
    bFinishedOnce = true;
    if (ensure(GetWorld()))
    {
        UHitStopSubsystem& HitStopSubsystem = UHitStopSubsystem::Get(GetWorld());
        HitStopSubsystem.UnregisterHitStop(this);
        
        if (TargetData.IsEmpty())
        {
            UE_LOG(LogCronus, Warning, TEXT("Cronus TargetData is empty in FinishHitStop"));
            return;
        }
        
        for (FHitStopTargetData& ItemToCancel : TargetData)
        {
            if (IsValid(ItemToCancel.Actor))
            {
                ItemToCancel.Actor->CustomTimeDilation = 1.f;
                if (ItemToCancel.Actor->GetClass()->ImplementsInterface(UHitStopInterface::StaticClass()))
                {
                    IHitStopInterface::Execute_OnHitStopCompleted(ItemToCancel.Actor);
                }
            }
            for (UNiagaraComponent* VFX : ItemToCancel.VFX)
            {
                if (IsValid(VFX))
                {
                    VFX->SetCustomTimeDilation(1.f);
                }
            }
            for (UHitStop_Cosmetic* Cosmetic : ItemToCancel.ActiveCosmetics)
            {
                if(ensure(IsValid(Cosmetic)))
                {
                    Cosmetic->EndEffect();
                }
            }
            ItemToCancel.ActiveCosmetics.Empty(); 
            UE_LOG(LogCronus, Log, TEXT("Finished hitstop for actor : %s"), *GetNameSafe(ItemToCancel.Actor))
        }
    }
    TargetData.Empty();
}


bool UHitStop::AffectsActor(AActor* Actor) const
{
    if(!IsActive())
    {
        return false;
    }
    if (!(IsValid(Actor)))
    {
        return false;
    }
    for (auto& SingleTargetData : TargetData)
    {
        if (IsValid(SingleTargetData.Actor))
        {
            if(SingleTargetData.Actor == Actor)
            {
                return true;
            }
        }
    }
    return false;
}


bool UHitStop::AffectsTargets(TArray<FHitStopTargetData> Targets) const
{
    for (auto& SingleTargetData : Targets)
    {
        if (TargetData.Contains(SingleTargetData)) // this will only check if the actors match, regardless of other variables
        {
            return true;
        }
    }
    return false;
}


void UHitStop::CancelHitStop()
{
    HitStopCancelled.ExecuteIfBound();
    Cleanup();
}


float UHitStop::GetTimePassed() const
{
    if (const UWorld* World = GetWorld())
    {
       return World->GetTimeSeconds() - StartTime;
    }
    return 0.f;
}


float UHitStop::GetDurationSeconds() const
{
    switch (DurationType)
    {
    case EHitstopDurationType::FrameBased:
        return Duration * GetWorld()->GetDeltaSeconds();
    case EHitstopDurationType::TimeBased:
        return Duration;
    }
    return 0.f;
}


UWorld* UHitStop::GetWorld() const
{
    // Outer is set when creating action via NewObject<T>
    if (GetOuter())
    {
        return GetOuter()->GetWorld();
    }
    return nullptr;
}


void UHitStop::UpdateCosmetics(float DeltaSeconds)
{
    if (bIsActive)
    {
        for (auto& SingleTargetData : TargetData)
        {
            if (IsValid(SingleTargetData.Actor))
            {
                for (auto& Cosmetic : SingleTargetData.ActiveCosmetics) 
                {
                    if(ensure(IsValid(Cosmetic)))
                    {
                        Cosmetic->Update(DeltaSeconds);
                    }
                }
            }
        }
    }
}


void UHitStop::OnTick(float DeltaSeconds)
{
    if (ensureAlways(bIsActive))
    {
        if (DurationType == EHitstopDurationType::FrameBased)
        {
            if (TickCounter < Duration)
            {
                // count how many frames have passed
                ++TickCounter;
            }
            else
            {
                // if enough frames have passed through, conclude the hitstop
                HitStopCompleted.ExecuteIfBound();
                Cleanup();
                return; // end early to avoid calling updatecosmetics
            }
        }
        else if (DurationType == EHitstopDurationType::TimeBased)
        {
            if(!OngoingDelay.IsValid() || !GetWorld()->GetTimerManager().IsTimerActive(OngoingDelay))
            {
                return; //skip update if timer has just been turned off
            }
        }
        UpdateCosmetics(DeltaSeconds);
    }
}
