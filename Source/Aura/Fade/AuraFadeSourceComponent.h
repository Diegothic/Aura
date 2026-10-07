// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AuraFadeSourceComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class AURA_API UAuraFadeSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAuraFadeSourceComponent();

	UFUNCTION(BlueprintCallable)
	void PushActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void PopActor(AActor* InActor);

private:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> FadedActors;
};
