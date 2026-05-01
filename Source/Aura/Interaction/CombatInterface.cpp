// Copyright Diegothic


#include "CombatInterface.h"


int32 ICombatInterface::GetCharacterLevel() const
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation() const
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
