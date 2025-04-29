// Copyright Diegothic


#include "AuraGameplayEventAnimNotify.h"

#include "AbilitySystemBlueprintLibrary.h"


void UAuraGameplayEventAnimNotify::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* const OwnerActor = MeshComp->GetOwner();
	const FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, EventTag, EventData);
}
