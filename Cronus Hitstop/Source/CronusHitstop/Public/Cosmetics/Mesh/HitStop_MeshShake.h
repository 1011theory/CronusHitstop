// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cosmetics/Mesh/HitStop_MeshCosmetic.h"

#include "HitStop_MeshShake.generated.h"


/**
 * Periodically shakes the main mesh of the hitstopped actor.
 */
UCLASS(Blueprintable, Abstract)
class CRONUSHITSTOP_API UHitStop_MeshShake : public UHitStop_MeshCosmetic
{
	GENERATED_BODY()

	UHitStop_MeshShake(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
protected:
	/* Shakes the actor's mesh by alternating its location between this value and the opposite of this value, at a "Frequency" 
	amount of times per second. The Z value is only used if the target is in air or if bOnlyShakeZAxisWhenInAir = false. */
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	FVector ShakeAmount = { 0.f, 0.f, 0.f };

	/* Max amount of random noise in units to add on top of the mesh shake. Useful to make the shake look more organic.*/
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	float Noise = 0.f;

	/* Typically, we only want to shake the target mesh on the Z axis if the target is in air. Only actors with movement 
	components that inherit from UPawnMovementComponent are checked for the IsInAir condition. */
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	bool bOnlyShakeZAxisWhenInAir = true;

	
public:
	virtual void StartEffect(AActor* Actor) override;
	virtual void EndEffect() override;


private:
	/* Default relative location of mesh, used to restore the mesh on finished. */
	FVector MeshDefaultLocation;

	/* To track last direction of shake movement and invert it on the next PeriodicEffect update. */
	bool bFlipFlop = false;

	
protected:
	/* Moves the mesh according to the ShakeAmount at the Frequency rate. */
	virtual void UpdateEffect_Internal() override;

	virtual void Initialize() override;

	/* Whether the Mesh Shake should change intensity based on how far the target is from the camera */
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	bool bIsAffectedByDistance = false;

	/* Adjust the shake intensity based on distance from the camera, e.g. If the shake looks correct
	when the camera is 1000 units from the target, but it looks too small when 3000 units away, we can increase the curve value
	at 3000 units until the shake intensity is right. */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bIsAffectedByDistance", EditConditionHides), Category = "Shake")
	FRuntimeFloatCurve DistanceIntensityCurve; // an interesting way to do this would be to look at screen space.

	
private:
	float GetDistanceMultiplier() const;
	
	FVector MakeNoise() const;

	bool IsTargetInAir() const;

	
	
};
