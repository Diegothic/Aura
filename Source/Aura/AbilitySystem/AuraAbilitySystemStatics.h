// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemStatics.generated.h"

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

private:
	static bool CreateWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams);
};
