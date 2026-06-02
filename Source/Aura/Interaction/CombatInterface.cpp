// Copyright Diegothic


#include "CombatInterface.h"


int32 ICombatInterface::GetCharacterLevel() const
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation_Implementation() const
{
	return FVector::ZeroVector;
}

void ICombatInterface::SetFacingTagetLocation_Implementation(const FVector& TargetLocation)
{
}

UAnimMontage* ICombatInterface::GetHitReactMontage_Implementation() const
{
	return nullptr;
}

void ICombatInterface::Die()
{
}

bool ICombatInterface::IsDead_Implementation() const
{
	return false;
}

AActor* ICombatInterface::GetAvatarActor_Implementation()
{
	return nullptr;
}
