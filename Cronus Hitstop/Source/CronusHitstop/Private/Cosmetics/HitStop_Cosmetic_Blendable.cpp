// Copyright The First Ones, All Rights Reserved.


#include "Cosmetics/HitStop_Cosmetic_Blendable.h"

//#include "HitStop.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitStop_Cosmetic_Blendable)

UHitStop_Cosmetic_Blendable::UHitStop_Cosmetic_Blendable(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	IntensityCurve.EditorCurveData.AddKey(0.0f, 1.0f);
	IntensityCurve.EditorCurveData.AddKey(1.0f, 1.0f);
}

float UHitStop_Cosmetic_Blendable::GetIntensityAtCurrentTime() const
{
	/*if(const UHitStop* HitStopRef = Cast<UHitStop>(GetOuter()))
	{*/
		//const float Alpha = HitStopRef->GetTimePassed() / FMath::Max(HitStopRef->GetDurationSeconds(), KINDA_SMALL_NUMBER); // To avoid division by 0;
		const float Alpha = GetTimePassed() / FMath::Max(Duration, KINDA_SMALL_NUMBER); // To avoid division by 0;
		return IntensityCurve.GetRichCurveConst()->Eval(Alpha);
	/*}
	return 0.f;*/
}

//@ToDo: replace intensity get with new generic curve function