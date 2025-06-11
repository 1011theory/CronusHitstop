#pragma once

#include "AssetTypeActions/AssetTypeActions_Blueprint.h"


class CRONUSHITSTOPEDITOR_API FAssetTypeActions_Cronus: public FAssetTypeActions_Blueprint
{
	const EAssetTypeCategories::Type AssetCategory;
	const FText Name;
	const FColor Color;

	
public:
	explicit FAssetTypeActions_Cronus(EAssetTypeCategories::Type AssetCategory, const FText& Name, const FColor& Color = {255, 180, 0})
		: AssetCategory(AssetCategory), Name(Name), Color(Color)
	{		
	}
	
	virtual FText GetName() const override; //{ return Name;}
	virtual FColor GetTypeColor() const override { return Color; }
	virtual UClass* GetSupportedClass() const override;// { return UHitStop_Cosmetic::StaticClass();}
	virtual uint32 GetCategories() override { return AssetCategory; }


protected:
	// FAssetTypeActions_Blueprint
	virtual bool CanCreateNewDerivedBlueprint() const override { return false; }
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;
};
