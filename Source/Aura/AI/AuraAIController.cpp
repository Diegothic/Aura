// Copyright Diegothic


#include "AuraAIController.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


AAuraAIController::AAuraAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	if (IsValid(BlackboardComponent))
	{
		Blackboard = BlackboardComponent;
	}

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behavior Tree Component");
	if (IsValid(BehaviorTreeComponent))
	{
		BrainComponent = BehaviorTreeComponent;
	}
}
