// Copyright Diegothic


#include "AuraEnemyCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraEnemyCharacter::AAuraEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	bHighlighted = false;
}

void AAuraEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitAbilityActorInfo();
}

void AAuraEnemyCharacter::HighlightActor()
{
	bHighlighted = true;

	USkeletalMeshComponent* const MeshComponent = GetMesh();
	MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(DepthStencilValueOutlineRed);

	USkeletalMeshComponent* const WeaponMesh = GetWeapon();
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(DepthStencilValueOutlineRed);
}

void AAuraEnemyCharacter::UnHighlightActor()
{
	bHighlighted = false;

	USkeletalMeshComponent* const MeshComponent = GetMesh();
	MeshComponent->SetRenderCustomDepth(false);
	MeshComponent->SetCustomDepthStencilValue(0);

	USkeletalMeshComponent* const WeaponMesh = GetWeapon();
	WeaponMesh->SetRenderCustomDepth(false);
	WeaponMesh->SetCustomDepthStencilValue(0);
}

int32 AAuraEnemyCharacter::GetCharacterLevel() const
{
	return Level;
}

void AAuraEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* const AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AbilityActorInfoSet();
}
