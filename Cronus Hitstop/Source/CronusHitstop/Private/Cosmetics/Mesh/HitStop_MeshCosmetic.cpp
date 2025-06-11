// Copyright The First Ones, 2024. All Rights Reserved.


#include "Cosmetics/Mesh/HitStop_MeshCosmetic.h"

#include "CronusHitstop.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_MeshCosmetic)

void UHitStop_MeshCosmetic::SetMeshType()
{
	if (IsValid(TargetActor))
	{
		if (const ACharacter* Character = Cast<ACharacter>(TargetActor))
		{
			if (CharacterFallbackMeshTag.IsValid() && Character->ActorHasTag(CharacterFallbackMeshTag))
			{
				TArray<UActorComponent*> Components = TargetActor->GetComponentsByTag(UMeshComponent::StaticClass(), FallbackMeshTag);

				if(Components.IsValidIndex(0))
				{
					if (UMeshComponent* GenericMesh = Cast<UMeshComponent>(Components[0]))
					{
						TargetMesh = GenericMesh;
						return;

					}
				}
			}
			TargetMesh = Character->GetMesh();
			return;
		}
		// if target is not a character, we use the first tagged mesh encountered //what about the edge case where we want to shake something else on a character class?
		if (bUseFallbackMesh & !FallbackMeshTag.IsNone())
		{
			TArray<UActorComponent*> Components = TargetActor->GetComponentsByTag(UMeshComponent::StaticClass(), FallbackMeshTag);

			if(Components.IsValidIndex(0))
			{
				if (UMeshComponent* GenericMesh = Cast<UMeshComponent>(Components[0]))
				{
					TargetMesh = GenericMesh;
					return;
				}
			}
		}
		// if no fallback mesh is specified or found, we attempt to shake the first mesh found of the actor.
		if (UMeshComponent* GenericMesh = TargetActor->GetComponentByClass<UMeshComponent>())
		{
			TargetMesh = GenericMesh;
			return;
		}
		UE_LOG(LogCronus, Warning, TEXT("Attempted to configure hitstop mesh shake on actor : %s, but could not find any meshes on it"), *GetNameSafe(TargetActor));
	}
}


void UHitStop_MeshCosmetic::StartEffect(AActor* Actor)
{
	TargetActor = Actor;
	SetMeshType();
	
	Super::StartEffect(Actor);
}


void UHitStop_MeshCosmetic::EndEffect()
{
	TargetActor = nullptr;
	TargetMesh = nullptr;
	Super::EndEffect();
}
