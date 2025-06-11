// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Hitstop_Types.generated.h"

UENUM()
enum class EHitstopDurationType : uint8 
{
	/* Time based hitstops will use the world time in seconds as their duration. Duration = 5 -> Stop for 5 seconds.*/
	TimeBased,

	/* Frame based hitstops will freeze targets for the defined amount of frames. Duration = 5 -> Stop for 5 frames. */
	FrameBased
};

//@ToDo: Bring Hitstop Structures in here.

