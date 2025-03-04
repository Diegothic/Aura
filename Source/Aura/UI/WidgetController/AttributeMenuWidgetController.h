// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"

#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeInfoSignature, const FAuraAttributeInfo&, Info, float, Value);

UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//~ Begin UAuraWidgetController Interface
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController Interface

public:
	UPROPERTY(BlueprintAssignable, Category = "Aura|AbilitySystem|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|AbilitySystem|Attributes",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
