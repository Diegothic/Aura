// Copyright Diegothic


#include "AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* const AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	TSet<FGameplayTag> AttributeTags;
	AttributeTags.Reserve(16);
	AuraAttributeSet->GetAttributeTags(AttributeTags);

	check(AttributeInfo);
	for (const FGameplayTag& AttributeTag : AttributeTags)
	{
		if (const TOptional<const FAuraAttributeInfo*> Info = AttributeInfo->FindAttributeInfo(AttributeTag))
		{
			float Value = 0.0f;
			if (
				const TOptional<TStaticFuncPtr<FGameplayAttribute()>> Attribute
					= AuraAttributeSet->FindGameplayAttributeGetter(AttributeTag)
			)
			{
				const TStaticFuncPtr<FGameplayAttribute()> AttributeGetter = Attribute.GetValue();
				Value = AttributeGetter().GetNumericValue(AuraAttributeSet);
			}

			AttributeInfoDelegate.Broadcast(*Info.GetValue(), Value);
		}
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}
