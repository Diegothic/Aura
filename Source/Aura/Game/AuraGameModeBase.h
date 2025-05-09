// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	const UCharacterClassInfo& GetCharacterClassInfo() const
	{
		check(CharacterClassInfo);
		return *CharacterClassInfo;
	}

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|CharacterClassDefaults",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
