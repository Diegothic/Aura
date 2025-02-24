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

void AAuraCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level) const
{
	UAbilitySystemComponent* const ASC = GetAbilitySystemComponent();
	if (!ASC || !EffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(
		EffectClass,
		Level,
		EffectContextHandle
	);

	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, ASC);
}
