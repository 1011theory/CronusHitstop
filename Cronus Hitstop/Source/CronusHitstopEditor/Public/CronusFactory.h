// Copyright The First Ones, 2024. All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "Cosmetics/HitStop_Cosmetic.h"
#include "CronusFactory.generated.h"

/**
 * 
 */
UCLASS()
class CRONUSHITSTOPEDITOR_API UCronusFactory : public UFactory
{
	GENERATED_BODY()

	UCronusFactory(const FObjectInitializer& ObjectInitializer);

public:
	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = CronusFactory)
	TSubclassOf<class UHitStop_Cosmetic> ParentClass;

private:
	// UFactory
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ConfigureProperties() override;
	// --
};


