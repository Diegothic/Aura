// Copyright Diegothic


#include "AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/TargetInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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
	const float InputAxisVectorLength = InputAxisVector.Length();
	if (InputAxisVectorLength < KINDA_SMALL_NUMBER)
	{
		return;
	}

	const FRotator Rotation(0.0, GetControlRotation().Yaw, 0.0);
	const FRotationMatrix RotationMatrix(Rotation);
	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	const FVector MoveVector = (ForwardVector * InputAxisVector.Y + RightVector * InputAxisVector.X)
		/ InputAxisVectorLength;
	const float MovementScale = FMath::Min(InputAxisVectorLength, 1.0f);

	if (APawn* const ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(
			MoveVector,
			MovementScale
		);
	}
}

void AAuraPlayerController::CursorTrace()
{
	LastTarget = CurrentTarget;

	// Update current target
	FHitResult CursorHit;
	const bool bHit = GetHitResultUnderCursor(ECC_Camera, false, CursorHit)
		&& CursorHit.bBlockingHit;
	AActor* const HitActor = bHit ? CursorHit.GetActor() : nullptr;
	CurrentTarget = HitActor && HitActor->Implements<UTargetInterface>()
		                ? TScriptInterface<ITargetInterface>(HitActor)
		                : nullptr;

	// Update highlight
	if (CurrentTarget != LastTarget)
	{
		if (LastTarget)
		{
			LastTarget->UnHighlightActor();
		}
		if (CurrentTarget)
		{
			CurrentTarget->HighlightActor();
		}
	}
}
