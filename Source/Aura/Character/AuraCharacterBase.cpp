// Copyright Diegothic


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	if (UCharacterMovementComponent* const MovementComp = GetCharacterMovement(); IsValid(MovementComp))
	{
		MovementComp->bUseControllerDesiredRotation = true;
	}

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

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	if (!Weapon || !Weapon->DoesSocketExist(WeaponSpellSocket))
	{
		return GetActorLocation();
	}

	return Weapon->GetSocketLocation(WeaponSpellSocket);
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	if (IsValid(Weapon))
	{
		const FDetachmentTransformRules WeaponDetachmentRules{
			EDetachmentRule::KeepWorld,
			true
		};
		Weapon->DetachFromComponent(WeaponDetachmentRules);
	}

	Multicast_HandleDeath();
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

void AAuraCharacterBase::GiveStartupAbilities() const
{
	if (!HasAuthority())
	{
		return;
	}

	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->GiveAbilities(StartupAbilities);
	}
}

void AAuraCharacterBase::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> DynamicMaterialsToDissolve;

	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* const DissolveMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
			DissolveMaterialInstance, this);
		DynamicMaterialsToDissolve.Emplace(DissolveMaterialInstanceDynamic);

		if (UMeshComponent* const MeshComp = GetMesh(); IsValid(MeshComp))
		{
			MeshComp->SetMaterial(0, DissolveMaterialInstanceDynamic);
		}
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* const WeaponDissolveMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		DynamicMaterialsToDissolve.Emplace(WeaponDissolveMaterialInstanceDynamic);

		if (UMeshComponent* const WeaponMeshComp = Weapon; IsValid(WeaponMeshComp))
		{
			WeaponMeshComp->SetMaterial(0, WeaponDissolveMaterialInstanceDynamic);
		}
	}

	BP_StartDissolveTimeline(DynamicMaterialsToDissolve);
}

void AAuraCharacterBase::Multicast_HandleDeath_Implementation()
{
	if (IsValid(Weapon))
	{
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	if (UMeshComponent* const MeshComp = GetMesh())
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetEnableGravity(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MeshComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}

	if (UCapsuleComponent* const CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	Dissolve();
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
