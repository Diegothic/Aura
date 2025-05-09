// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemStatics.generated.h"

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

private:
	static bool CreateWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams);
};
