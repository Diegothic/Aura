// Copyright Diegothic


#include "AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* const AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	TSet<FGameplayTag> AttributeTags;
	AttributeTags.Reserve(16);
	AuraAttributeSet->GetAttributeTags(AttributeTags);
	for (const FGameplayTag& AttributeTag : AttributeTags)
	{
		const TOptional<const FAuraAttributeInfo*> Info = AttributeInfo->FindAttributeInfo(AttributeTag);
		const TOptional<TStaticFuncPtr<FGameplayAttribute()>> Attribute
			= AuraAttributeSet->FindGameplayAttributeGetter(AttributeTag);
		if (Info && Attribute)
		{
			const TStaticFuncPtr<FGameplayAttribute()> AttributeGetter = Attribute.GetValue();
			const float Value = AttributeGetter().GetNumericValue(AuraAttributeSet);

			AttributeInfoDelegate.Broadcast(*Info.GetValue(), Value);
		}
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const UAuraAttributeSet* const AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	TSet<FGameplayTag> AttributeTags;
	AttributeTags.Reserve(16);
	AuraAttributeSet->GetAttributeTags(AttributeTags);
	for (const FGameplayTag& AttributeTag : AttributeTags)
	{
		if (
			const TOptional<TStaticFuncPtr<FGameplayAttribute()>> Attribute
				= AuraAttributeSet->FindGameplayAttributeGetter(AttributeTag)
		)
		{
			const TStaticFuncPtr<FGameplayAttribute()> AttributeGetter = Attribute.GetValue();
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeGetter()).AddLambda(
				[this, AttributeTag](const FOnAttributeChangeData& Data)
				{
					if (
						const TOptional<const FAuraAttributeInfo*> Info
							= AttributeInfo->FindAttributeInfo(AttributeTag)
					)
					{
						AttributeInfoDelegate.Broadcast(*Info.GetValue(), Data.NewValue);
					}
				}
			);
		}
	}
}
