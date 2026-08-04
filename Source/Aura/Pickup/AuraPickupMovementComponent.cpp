// Copyright Diegothic


#include "AuraPickupMovementComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"


UAuraPickupMovementComponent::UAuraPickupMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAuraPickupMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AActor* const Owner = GetOwner(); IsValid(Owner))
	{
		InitialActorLocation = Owner->GetActorLocation();
		InitialActorRotation = Owner->GetActorRotation();
	}
}

void UAuraPickupMovementComponent::TickComponent(
	const float InDeltaTime,
	const ELevelTick InTickType,
	FActorComponentTickFunction* InThisTickFunction
)
{
	Super::TickComponent(InDeltaTime, InTickType, InThisTickFunction);

	if (const UWorld* const World = GetWorld())
	{
		const double WorldTimeSeconds = World->GetTime().GetWorldTimeSeconds();

		const float FloatingOffset = FMath::Sin(WorldTimeSeconds * FloatingSpeed * PI) * MaxFloatingOffset;
		const FVector NewActorLocation = InitialActorLocation + FVector::UpVector * FloatingOffset;

		const float RotationYaw = WorldTimeSeconds * RotationSpeed;
		const FQuat DeltaQuat = FQuat{FVector::UpVector, FMath::DegreesToRadians(RotationYaw)};
		const FRotator NewActorRotation = (DeltaQuat * InitialActorRotation.Quaternion()).Rotator();

		if (AActor* const Owner = GetOwner(); IsValid(Owner))
		{
			Owner->SetActorLocation(NewActorLocation);
			Owner->SetActorRotation(NewActorRotation);
		}
	}
}
