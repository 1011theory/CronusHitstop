// Copyright The First Ones, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "HitStop_CosmeticBlueprint.generated.h"

/**
 * A blueprintable Cosmetic to apply during a Hitstop.
 */
UCLASS(BlueprintType)
class CRONUSHITSTOP_API UHitStop_CosmeticBlueprint : public UBlueprint
{
	GENERATED_BODY()

#if WITH_EDITOR
	// UBlueprint
	virtual bool SupportedByDefaultBlueprintFactory() const override { return false; }
	virtual bool SupportsDelegates() const override { return false; }
	// --
#endif
};
