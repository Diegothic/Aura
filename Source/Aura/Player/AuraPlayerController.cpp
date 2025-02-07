// Copyright Diegothic


#include "AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputMappingContext);

	if (const ULocalPlayer* const LocalPlayer = GetLocalPlayer())
	{
		if (
			UEnhancedInputLocalPlayerSubsystem* const EnhancedInputSubsystem
				= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)
		)
		{
			EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(
		MoveInputAction,
		ETriggerEvent::Triggered,
		this,
		&AAuraPlayerController::OnMoveTriggered
	);
}

void AAuraPlayerController::OnMoveTriggered(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	const FRotator Rotation(0.0, GetControlRotation().Yaw, 0.0);
	const FRotationMatrix RotationMatrix(Rotation);
	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	if (APawn* const ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}
