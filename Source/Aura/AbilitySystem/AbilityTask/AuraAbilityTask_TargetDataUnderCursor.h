// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AuraAbilityTask_TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderCursorDeleagate,
                                            const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class AURA_API UAuraAbilityTask_TargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderCursorDeleagate OnDataAcquired;

public:
	UFUNCTION(BlueprintCallable, Category="Aura|Ability|Tasks", meta = (DisplayName="TargetDataUnderCursor",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAuraAbilityTask_TargetDataUnderCursor* CreateTargetDataUnderCursorProxy(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName
	);

	virtual void Activate() override;

private:
	void SendCursorData() const;
	void OnTargetDataSetCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const;
};
