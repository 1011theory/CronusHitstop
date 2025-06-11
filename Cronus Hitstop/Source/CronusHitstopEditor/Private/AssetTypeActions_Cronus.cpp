#include "AssetTypeActions_Cronus.h"

#include "CronusFactory.h"
#include "Cosmetics/HitStop_CosmeticBlueprint.h"
#include "Cosmetics/HitStop_Cosmetic.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions_Cronus"

FText FAssetTypeActions_Cronus::GetName() const
{
	return LOCTEXT("AssetTypeActions_Cronus", "Hitstop Cosmetic");
}

UClass* FAssetTypeActions_Cronus::GetSupportedClass() const
{
	return UHitStop_CosmeticBlueprint::StaticClass();
}

UFactory* FAssetTypeActions_Cronus::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UCronusFactory* CronusFactory = NewObject<UCronusFactory>();
	CronusFactory->ParentClass = TSubclassOf<UHitStop_Cosmetic>(*InBlueprint->GeneratedClass);
	return CronusFactory;
}

#undef LOCTEXT_NAMESPACE