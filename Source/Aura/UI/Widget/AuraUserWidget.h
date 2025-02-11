// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "AuraUserWidget.generated.h"

UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Aura|Widget")
	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura|Widget")
	void OnWidgetControllerSet();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UObject> WidgetController;
};
