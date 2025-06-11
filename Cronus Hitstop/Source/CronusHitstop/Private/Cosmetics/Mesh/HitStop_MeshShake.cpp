// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/Mesh/HitStop_MeshShake.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_MeshShake)

UHitStop_MeshShake::UHitStop_MeshShake(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	DistanceIntensityCurve.EditorCurveData.AddKey(500, 1);
	DistanceIntensityCurve.EditorCurveData.AddKey(3000, 1);
	//bTriggerAtStart = false;
}


void UHitStop_MeshShake::Initialize()
{
	Super::Initialize();
	
	if(ensure(TargetMesh))
	{
		check(!bIsActive);
		MeshDefaultLocation = TargetMesh->GetRelativeLocation();
		bIsActive = true;
	}
}


void UHitStop_MeshShake::StartEffect(AActor* Actor)
{
	Super::StartEffect(Actor);
}


void UHitStop_MeshShake::EndEffect()
{
	if (ensure(TargetMesh))
	{
		TargetMesh->SetRelativeLocation(MeshDefaultLocation);
		TargetMesh = nullptr;
	}
	Super::EndEffect();
}


void UHitStop_MeshShake::UpdateEffect_Internal()
{
	if(ensure(bIsActive))
	{
		Super::UpdateEffect_Internal();

		if (ensure(TargetMesh) && ensure(!TargetMesh->IsSimulatingPhysics())) // Warn even if PIE will warn anyway?
		{
			FVector AdjustedShakeAmount = ShakeAmount + MakeNoise();
			AdjustedShakeAmount = AdjustedShakeAmount * (bUseIntensityCurve ? GetIntensityAtCurrentTime() : 1.f);
			AdjustedShakeAmount.Z = IsTargetInAir() ? AdjustedShakeAmount.Z : 0;
		
			if (bIsAffectedByDistance)
			{
				AdjustedShakeAmount = AdjustedShakeAmount * GetDistanceMultiplier();
			}
			
			AdjustedShakeAmount *= Scale;
			
			if (bFlipFlop == false)
			{
				TargetMesh->SetRelativeLocation(MeshDefaultLocation+AdjustedShakeAmount);
				bFlipFlop = true;
			}
			else
			{
				TargetMesh->SetRelativeLocation(MeshDefaultLocation-AdjustedShakeAmount);
				bFlipFlop = false;
			}
		}
	}
}


float UHitStop_MeshShake::GetDistanceMultiplier() const
{
	if (const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetOuter(), 0))
	{
		if (TargetMesh)
		{
			const FVector CameraLocation = CameraManager->GetCameraLocation();
			const FVector MeshLocation = TargetMesh->GetComponentLocation();
			const float Distance = UKismetMathLibrary::Vector_Distance(CameraLocation, MeshLocation);
			return DistanceIntensityCurve.GetRichCurveConst()->Eval(Distance);
		}
	}
	return 1.f;
}


FVector UHitStop_MeshShake::MakeNoise() const
{
	FVector FinalNoise = FVector::ZeroVector;
	
	if (Noise != 0.f)
	{
		// We  only calculate noise if the default shake amount isn't 0 for the given axis.
		FinalNoise.X = ShakeAmount.X == 0.f ? 0 : UKismetMathLibrary::RandomFloatInRange(-Noise, Noise);
		FinalNoise.Y = ShakeAmount.Y == 0.f ? 0 : UKismetMathLibrary::RandomFloatInRange(-Noise, Noise);
		FinalNoise.Z = ShakeAmount.Z == 0.f ? 0 : UKismetMathLibrary::RandomFloatInRange(-Noise, Noise);
	}

	return FinalNoise;
}


bool UHitStop_MeshShake::IsTargetInAir() const
{
	if (IsValid(TargetActor))
	{
		if (const UPawnMovementComponent* MovementComponent = TargetActor->GetComponentByClass<UPawnMovementComponent>())
		{
			return !MovementComponent->IsMovingOnGround();
		}
	}
	return false;
}

