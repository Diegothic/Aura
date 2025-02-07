// Copyright Diegothic


#include "AuraPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
