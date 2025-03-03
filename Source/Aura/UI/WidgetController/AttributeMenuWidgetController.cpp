// Copyright Diegothic


#include "AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameplayTags/AuraGameplayTags.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* const AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);
	if (
		TOptional<const FAuraAttributeInfo*> Info
			= AttributeInfo->FindAttributeInfo(UAuraGameplayTags::Get().Attribute_Primary_Strength)
	)
	{
		float StrengthValue = AuraAttributeSet->GetStrength();
		AttributeInfoDelegate.Broadcast(*Info.GetValue(), StrengthValue);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}
