// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"


UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(
		const UAuraInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc,
		HeldFuncType HeldFunc
	);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(
	const UAuraInputConfig* InputConfig,
	UserClass* Object,
	PressedFuncType PressedFunc,
	ReleasedFuncType ReleasedFunc,
	HeldFuncType HeldFunc
)
{
	check(InputConfig);

	for (const FAuraInputAction& Action : InputConfig->GetAbilityInputActions())
	{
		if (!Action.InputAction || !Action.InputTag.IsValid())
		{
			continue;
		}

		if (PressedFunc)
		{
			BindAction<UserClass, FGameplayTag>(
				Action.InputAction,
				ETriggerEvent::Started,
				Object,
				PressedFunc,
				Action.InputTag
			);
		}

		if (ReleasedFunc)
		{
			BindAction<UserClass, FGameplayTag>(
				Action.InputAction,
				ETriggerEvent::Completed,
				Object,
				ReleasedFunc,
				Action.InputTag
			);
		}

		if (HeldFunc)
		{
			BindAction<UserClass, FGameplayTag>(
				Action.InputAction,
				ETriggerEvent::Triggered,
				Object,
				HeldFunc,
				Action.InputTag
			);
		}
	}
}
