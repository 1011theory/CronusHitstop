// Copyright The First Ones, 2024. All Rights Reserved.


#include "CronusFactory.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#include "BlueprintEditorSettings.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "Editor.h"

#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Cosmetics/HitStop_CosmeticBlueprint.h"



#define LOCTEXT_NAMESPACE "CronusFactory"

class FHitStop_CosmeticBlueprintParentFilter final : public IClassViewerFilter
{
public:
	/** All children of these classes will be included unless filtered out by another setting. */
	TSet<const UClass*> AllowedChildrenOfClasses;
	
	FHitStop_CosmeticBlueprintParentFilter() {}
	
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		const bool bAllowed = !InClass->HasAnyClassFlags(CLASS_Hidden) && InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
			
		return bAllowed;
	}
	
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		// If it appears on the allowed child-of classes list (or there is nothing on that list)
		return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};


//--------------------------------------------------------------------------------------------------------------------------


UCronusFactory::UCronusFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UHitStop_CosmeticBlueprint::StaticClass(); //@ToDo: Delete?
	ParentClass = UHitStop_Cosmetic::StaticClass();
	
	bCreateNew = true;
	bEditAfterNew = true;
}


bool UCronusFactory::ConfigureProperties()
{
	// const TSharedRef<SHitStop_CosmeticBlueprintCreateDialog> Dialog = SNew(SHitStop_CosmeticBlueprintCreateDialog);
	// return Dialog->ConfigureProperties(this);

	//ParentClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	Options.bIsBlueprintBaseOnly = true;
	Options.bExpandRootNodes = true;
	
	const TSharedPtr<FHitStop_CosmeticBlueprintParentFilter> Filter = MakeShareable(new FHitStop_CosmeticBlueprintParentFilter);

	// All child classes of Hitstop_Cosmetic are valid
	Filter->AllowedChildrenOfClasses.Add(UHitStop_Cosmetic::StaticClass());
	Options.ClassFilters = {Filter.ToSharedRef()};

	const FText TitleText = LOCTEXT("CreateItemOptions", "Pick Hitstop Cosmetic Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UHitStop_Cosmetic::StaticClass());

	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}


UObject* UCronusFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UHitStop_CosmeticBlueprint::StaticClass()));

	if (ParentClass == nullptr || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass) || !ParentClass->IsChildOf(UHitStop_Cosmetic::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), ParentClass ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotCreateHitStop_CosmeticBlueprint", "Cannot create a Hitstop Cosmetic Blueprint based on the chosen class."), Args));
		return nullptr;
	}
	
	UHitStop_CosmeticBlueprint* NewBP = CastChecked<UHitStop_CosmeticBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BPTYPE_Normal, UHitStop_CosmeticBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
	
	return NewBP;
}


UObject* UCronusFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

#undef LOCTEXT_NAMESPACE