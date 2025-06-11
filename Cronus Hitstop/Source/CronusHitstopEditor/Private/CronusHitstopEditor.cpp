// Copyright The First Ones, 2024. All Rights Reserved.

#include "CronusHitstopEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTypeActions_Cronus.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCronusHitstopEditorModule" 

TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
void FCronusHitstopEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	StyleSet = MakeShareable(new FSlateStyleSet("CronusStyle"));
	const FString Path = IPluginManager::Get().FindPlugin("CronusHitstop")->GetBaseDir()/ TEXT("Resources");
	StyleSet->Set("ClassThumbnail.HitStop_Cosmetic", new FSlateImageBrush(Path + "/HitStop_ClassIcon_128.png", FVector2D(128, 128)));
	StyleSet->Set("ClassThumbnail.HitStop_CosmeticBlueprint", new FSlateImageBrush(Path + "/HitStop_ClassIcon_128.png", FVector2D(128, 128)));
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	
	///Asset Tools
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	CustomObjectCategory = AssetTools.RegisterAdvancedAssetCategory(FName("Hitstop"), FText::FromString("Hitstop"));
	Internal_RegisterTypeActions<FAssetTypeActions_Cronus>("Hitstop Cosmetic");
}


void FCronusHitstopEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	
	///Asset Tools
	if (const FAssetToolsModule* AssetToolsModulePtr = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		IAssetTools& AssetTools = AssetToolsModulePtr->Get();
		for (const auto& Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCronusHitstopEditorModule, CronusHitstopEditor);
