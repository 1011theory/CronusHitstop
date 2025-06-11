// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "AssetTypeCategories.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"


class FCronusHitstopEditorModule : public IModuleInterface
{
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
	EAssetTypeCategories::Type CustomObjectCategory = EAssetTypeCategories::None;
	
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	
private:
	template<class T>
	void Internal_RegisterTypeActions(const FString& Name)
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		const auto Action = MakeShared<T>(CustomObjectCategory, FText::FromString(Name));
		RegisteredAssetTypeActions.Emplace(Action);
		AssetTools.RegisterAssetTypeActions(Action);
	}
};
