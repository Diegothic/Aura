// Copyright Diegothic


#include "AuraPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->bInheritRoll = false;

	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0, 540.0, 0.0);
	Movement->bConstrainToPlane = true;
	Movement->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info for the Server
	InitAbilityActorInfo();

	// Init default attribute values - only for the Server since it's replicated
	InitDefaultAttributes();

	// Init HUD for the Server
	InitHUD();
}

void AAuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info for the Client
	InitAbilityActorInfo();

	// Init HUD for the Client
	InitHUD();
}

void AAuraPlayerCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AAuraPlayerState* CurrentPlayerState = GetPlayerState<AAuraPlayerState>();
	check(CurrentPlayerState);
	AbilitySystemComponent = CurrentPlayerState->GetAbilitySystemComponent();
	AttributeSet = CurrentPlayerState->GetAttributeSet();
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(CurrentPlayerState, this);

	UAuraAbilitySystemComponent* const AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AbilityActorInfoSet();
}

void AAuraPlayerCharacter::InitHUD() const
{
	AAuraPlayerState* const CurrentPlayerState = GetPlayerState<AAuraPlayerState>();
	check(CurrentPlayerState);
	if (AAuraPlayerController* const CurrentPlayerController = GetController<AAuraPlayerController>())
	{
		if (AAuraHUD* const CurrentHUD = Cast<AAuraHUD>(CurrentPlayerController->GetHUD()))
		{
			CurrentHUD->Initialize(
				CurrentPlayerController,
				CurrentPlayerState,
				AbilitySystemComponent,
				AttributeSet
			);
		}
	}
}
