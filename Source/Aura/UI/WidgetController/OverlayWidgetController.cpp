// Copyright Diegothic


#include "OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	if (const UAuraAttributeSet* const AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	if (const UAuraAttributeSet* const AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
		                      .AddLambda([this](const FOnAttributeChangeData& Data)
		                      {
			                      OnHealthChanged.Broadcast(Data.NewValue);
		                      });
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
		                      .AddLambda([this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxHealthChanged.Broadcast(Data.NewValue);
		                      });

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
		                      .AddLambda([this](const FOnAttributeChangeData& Data)
		                      {
			                      OnManaChanged.Broadcast(Data.NewValue);
		                      });
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
		                      .AddLambda([this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxManaChanged.Broadcast(Data.NewValue);
		                      });

		UAuraAbilitySystemComponent* const AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		AuraASC->EffectAssetTagsDelegate.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(TEXT("Message"));
			for (const FGameplayTag& AssetTag : AssetTags)
			{
				if (AssetTag.MatchesTag(MessageTag))
				{
					if (
						const FUIWidgetRow* const DataRow = GetDataTableRowByTag<FUIWidgetRow>(
							MessageWidgetDataTable,
							AssetTag
						))
					{
						MessageWidgetRowDelegate.Broadcast(*DataRow);
					}
				}
			}
		});
	}
}
