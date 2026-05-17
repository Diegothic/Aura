// Copyright Diegothic


#include "AuraEnemyCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemStatics.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemyCharacter::AAuraEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	bHighlighted = false;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
}

void AAuraEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemStatics::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	BindToAbilitySystemEvents();
}

void AAuraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraUserWidget* const AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	BroadcastInitialAttributeValues();
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

void AAuraEnemyCharacter::Die()
{
	SetLifeSpan(DeathLifeSpan);
	Super::Die();
}

void AAuraEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* const AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	check(AuraASC);
	AuraASC->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitDefaultAttributes();
	}
}

void AAuraEnemyCharacter::InitDefaultAttributes() const
{
	UAuraAbilitySystemStatics::InitDefaultAttributesForClass(
		this,
		CharacterClass,
		Level,
		AbilitySystemComponent
	);
}

void AAuraEnemyCharacter::BindToAbilitySystemEvents()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (const UAuraAttributeSet* const AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(
		FAuraGameplayTags::Get().GameplayEffect_HitReact,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &ThisClass::OnHitReactTagChangedEvent);
}

void AAuraEnemyCharacter::BroadcastInitialAttributeValues() const
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (const UAuraAttributeSet* const AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemyCharacter::OnHitReactTagChangedEvent(const FGameplayTag InChangedTag, int32 InNewTagCount)
{
	bReactingToHit = InNewTagCount > 0;

	UCharacterMovementComponent* const MovementComp = GetCharacterMovement();
	if (IsValid(MovementComp))
	{
		if (bReactingToHit)
		{
			MovementComp->StopMovementImmediately();
		}
	}
}
