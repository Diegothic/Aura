// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeapon() const { return Weapon; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Combat", Meta = (AllowPrivateAccess = "true"))
	FName WeaponSocket;
};
