// Copyright Diegothic


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayTags/AuraGameplayTags.h"


UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
