// Copyright Diegothic


#include "AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameplayTags/AuraGameplayTags.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
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

	UAuraInputComponent* const AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(
		MoveInputAction,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::OnMoveTriggered
	);

	AuraInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::OnAbilityActionPressed,
		&ThisClass::OnAbilityActionReleased,
		&ThisClass::OnAbilityActionHeld
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

void AAuraPlayerController::OnAbilityActionPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		bIsTargeting = CurrentTarget != nullptr;
		bIsAutoRunning = false;
	}
}

void AAuraPlayerController::OnAbilityActionReleased(FGameplayTag InputTag)
{
	if (UAuraAbilitySystemComponent* const ASC = GetAuraASC())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AAuraPlayerController::OnAbilityActionHeld(FGameplayTag InputTag)
{
	if (bIsTargeting || !InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		if (UAuraAbilitySystemComponent* const ASC = GetAuraASC())
		{
			ASC->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		CursorTraceTime += GetWorld()->GetDeltaSeconds();
		FHitResult CursorHit;
		if (GetHitResultUnderCursor(ECC_Camera, false, CursorHit))
		{
			CachedDestination = CursorHit.Location;
		}

		if (CursorTraceTime > ShortPressThreshold)
		{
			if (APawn* const ControlledPawn = GetPawn<APawn>())
			{
				const FVector PawnLocation = ControlledPawn->GetActorLocation();
				const FVector ToDestination = CachedDestination - PawnLocation;
				if (ToDestination.SizeSquared() > AutoRunAcceptanceRadius * AutoRunAcceptanceRadius)
				{
					const FVector MovementDirection = (CachedDestination - PawnLocation).GetSafeNormal();
					ControlledPawn->AddMovementInput(MovementDirection);
				}
			}
		}
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

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (!AuraAbilitySystemComponent)
	{
		UAbilitySystemComponent* const ASC
			= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(ASC);
	}

	return AuraAbilitySystemComponent;
}
