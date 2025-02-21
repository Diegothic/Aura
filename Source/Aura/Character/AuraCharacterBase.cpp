// Copyright Diegothic


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AAuraCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMeshComponent* const CharacterMesh = GetMesh();
	if (Weapon && CharacterMesh)
	{
		Weapon->AttachToComponent(
			CharacterMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket
		);
	}
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitPrimaryAttributes() const
{
	UAbilitySystemComponent* const ASC = GetAbilitySystemComponent();
	if (!ASC || !DefaultPrimaryAttributes)
	{
		return;
	}

	const FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(
		DefaultPrimaryAttributes,
		1.0f,
		EffectContextHandle
	);

	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, ASC);
}
