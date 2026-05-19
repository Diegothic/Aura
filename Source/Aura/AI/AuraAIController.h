// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAuraAIController();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Aura|AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Aura|AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
