// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"

#include "AttributeMenuWidgetController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//~ Begin UAuraWidgetController Interface
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController Interface
};
