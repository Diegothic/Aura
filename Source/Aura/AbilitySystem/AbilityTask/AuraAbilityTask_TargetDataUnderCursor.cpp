// Copyright Diegothic


#include "AuraAbilityTask_TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"


UAuraAbilityTask_TargetDataUnderCursor* UAuraAbilityTask_TargetDataUnderCursor::CreateTargetDataUnderCursorProxy(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName
)
{
	UAuraAbilityTask_TargetDataUnderCursor* MyObj = NewAbilityTask<UAuraAbilityTask_TargetDataUnderCursor>(
		OwningAbility,
		TaskInstanceName
	);

	return MyObj;
}

void UAuraAbilityTask_TargetDataUnderCursor::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	const FGameplayAbilityActorInfo* const ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo->IsLocallyControlled())
	{
		SendCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
		                      .AddUObject(
			                      this,
			                      &ThisClass::OnTargetDataSetCallback
		                      );
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle,
			PredictionKey
		);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAuraAbilityTask_TargetDataUnderCursor::SendCursorData() const
{
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get());

	const FGameplayAbilityActorInfo* const ActorInfo = Ability->GetCurrentActorInfo();
	const APlayerController* const PlayerController = ActorInfo->PlayerController.Get();
	if (IsValid(PlayerController))
	{
		FHitResult CursorHit;
		const bool bHit = PlayerController->GetHitResultUnderCursor(
			ECC_Camera,
			false,
			CursorHit
		);
		if (bHit && CursorHit.bBlockingHit)
		{
			FGameplayAbilityTargetData_SingleTargetHit* const TargetData
				= new FGameplayAbilityTargetData_SingleTargetHit(CursorHit);

			FGameplayAbilityTargetDataHandle Handle;
			Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));

			if (!PlayerController->HasAuthority())
			{
				AbilitySystemComponent->ServerSetReplicatedTargetData(
					GetAbilitySpecHandle(),
					GetActivationPredictionKey(),
					Handle,
					FGameplayTag(),
					AbilitySystemComponent->ScopedPredictionKey
				);
			}

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnDataAcquired.Broadcast(Handle);
			}
		}
	}
}

void UAuraAbilityTask_TargetDataUnderCursor::OnTargetDataSetCallback(
	const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag
) const
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDataAcquired.Broadcast(DataHandle);
	}
}
