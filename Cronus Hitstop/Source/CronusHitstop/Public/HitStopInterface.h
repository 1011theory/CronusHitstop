// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HitStopInterface.generated.h"


UINTERFACE(MinimalAPI)
class UHitStopInterface : public UInterface
{
    GENERATED_BODY()
};

/* Interface to add custom rules to actors that receive hitstops. Can only be used on Actors. */
class CRONUSHITSTOP_API IHitStopInterface
{
    GENERATED_BODY()

public:
 
    /* Allows actors to define their custom rules when accepting/declining hit stop requests from the subsystem */
    UFUNCTION(BlueprintNativeEvent, Category = "HitStop")
    bool CanBeHitStopped(); //@ToDo: make true by default?

    /* Called after the Hitstop Subsystem starts a Hitstop for the actor that owns this interface */
    UFUNCTION(BlueprintNativeEvent, Meta = (ForceAsFunction), Category = "HitStop")
    void OnHitStopStarted();

    /* Called after the Hitstop Subsystem finishes a Hitstop for the actor that owns this interface */
    UFUNCTION(BlueprintNativeEvent, Meta = (ForceAsFunction), Category = "HitStop")
    void OnHitStopCompleted();

};