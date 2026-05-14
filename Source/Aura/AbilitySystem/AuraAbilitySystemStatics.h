// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemStatics.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;

UCLASS()
class AURA_API UAuraAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura|AbilitySystem|Statics")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura|AbilitySystem|Statics")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

public:
	UFUNCTION(BlueprintCallable, Category = "Aura|AbilitySystem|CharacterClassDefaults")
	static void InitDefaultAttributesForClass(
		const UObject* WorldContextObject,
		const ECharacterClass& CharacterClass,
		float CharacterLevel,
		UAbilitySystemComponent* DestASC
	);

	UFUNCTION(BlueprintCallable, Category = "Aura|AbilitySystem|CharacterClassDefaults")
	static void GiveStartupAbilities(
		const UObject* WorldContextObject,
		UAbilitySystemComponent* DestASC
	);

	UFUNCTION(BlueprintPure, Category = "Aura|AbilitySystem|CharacterClassDefaults")
	static const UCharacterClassInfo* GetCharacterClassInfo(const UObject* InWorldContextObject);

public:
	UFUNCTION(BlueprintPure, Category = "Aura|AbilitySystem|GameplayEffect")
	static bool GetIsBlockedHit(const FGameplayEffectContextHandle& InEffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura|AbilitySystem|GameplayEffect")
	static void SetIsBlockedHit(
		UPARAM(ref) FGameplayEffectContextHandle& InEffectContextHandle,
		bool bInIsBlockedHit
	);

	UFUNCTION(BlueprintPure, Category = "Aura|AbilitySystem|GameplayEffect")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& InEffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura|AbilitySystem|GameplayEffect")
	static void SetIsCriticalHit(
		UPARAM(ref) FGameplayEffectContextHandle& InEffectContextHandle,
		bool bInIsCriticalHit
	);

private:
	static bool CreateWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams);
};
