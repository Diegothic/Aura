// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AuraGameplayEventAnimNotify.generated.h"

UCLASS()
class AURA_API UAuraGameplayEventAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Aura|Event", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EventTag;
};
