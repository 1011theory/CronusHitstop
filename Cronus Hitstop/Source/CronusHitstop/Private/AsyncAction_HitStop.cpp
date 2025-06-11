// Copyright The First Ones, 2024. All Rights Reserved.

#include "AsyncAction_HitStop.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "HitStop.h"
#include "HitStopSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_HitStop)

UAsyncAction_HitStop* UAsyncAction_HitStop::AsyncAction_HitStop(const UObject* WorldContext, const TArray<FHitStopTargetData> TargetData, const float Duration, const float Scale, const EHitstopDurationType Type)
{
    // We must have a valid contextual world for this action, so we must be able to resolve the UWorld from WorldContext
    UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
    if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
    {
        return nullptr;
    }

    // Create node and store variables
    UAsyncAction_HitStop* Node = NewObject<UAsyncAction_HitStop>();
    Node->ContextWorld = ContextWorld;
    Node->ReceivedTargetData = TargetData;
    Node->Duration = Duration;
    Node->Scale = Scale;
    Node->Type = Type;
    Node->RegisterWithGameInstance(ContextWorld->GetGameInstance());
    return Node;
}


void UAsyncAction_HitStop::Activate()
{
    if (const UWorld* World = GetWorld())
    {
        UHitStopSubsystem& HitStopSubsystem = UHitStopSubsystem::Get(World);
        UHitStop* NewHitStop;
        HitStopSubsystem.SimpleHitStop_Internal(ReceivedTargetData, Duration, Scale, Type, NewHitStop);
        HitStopRef = NewHitStop;
        if(IsValid(HitStopRef))
        {
            if (HitStopRef->IsActive())
            {
                HitStopRef->HitStopCompleted.BindUObject(this, &UAsyncAction_HitStop::HitStopCompleted);
                HitStopRef->HitStopCancelled.BindUObject(this, &UAsyncAction_HitStop::HitStopCancelled);
                return;
            }
        }
    }
    HitStopCancelled();
}


void UAsyncAction_HitStop::Cancel()
{
    if (GetWorld())
    {
        ReceivedTargetData.Empty();
        if (HitStopRef)
        {
            HitStopRef->HitStopCompleted.Unbind();

            HitStopRef->HitStopCancelled.Unbind();
            
            HitStopRef = nullptr;
        }
    }
    Super::Cancel();
}


void UAsyncAction_HitStop::HitStopCompleted()
{
    OnComplete.Broadcast();
    Cancel();
}


void UAsyncAction_HitStop::HitStopCancelled()
{
    OnCancelled.Broadcast();
    Cancel();
}














