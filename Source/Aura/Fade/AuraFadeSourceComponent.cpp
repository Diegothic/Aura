// Copyright Diegothic


#include "AuraFadeSourceComponent.h"

#include "AuraFadeComponent.h"


UAuraFadeSourceComponent::UAuraFadeSourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAuraFadeSourceComponent::PushActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return;
	}

	if (FadedActors.Contains(InActor))
	{
		return;
	}

	UAuraFadeComponent* const FadeComp = InActor->GetComponentByClass<UAuraFadeComponent>();
	if (!IsValid(FadeComp))
	{
		return;
	}

	FadedActors.Emplace(InActor);
	FadeComp->FadeOut(this);
}

void UAuraFadeSourceComponent::PopActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return;
	}

	if (!FadedActors.Contains(InActor))
	{
		return;
	}

	UAuraFadeComponent* const FadeComp = InActor->GetComponentByClass<UAuraFadeComponent>();
	if (!IsValid(FadeComp))
	{
		return;
	}

	FadedActors.Remove(InActor);
	FadeComp->FadeIn(this);
}
