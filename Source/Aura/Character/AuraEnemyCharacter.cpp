// Copyright Diegothic


#include "AuraEnemyCharacter.h"


AAuraEnemyCharacter::AAuraEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bHighlighted = false;
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
