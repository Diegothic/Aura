// Copyright Diegothic


#include "AuraSummonGameplayAbility.h"

#include "NavigationSystem.h"


TArray<FVector> UAuraSummonGameplayAbility::FindSummonLocations() const
{
	TArray<FVector> Result;

	if (const AActor* const AvatarActor = GetAvatarActorFromActorInfo(); IsValid(AvatarActor))
	{
		const FVector AvatarLocation = AvatarActor->GetActorLocation();
		const FVector AvatarForward = AvatarActor->GetActorForwardVector();

		const float StepAngle_Deg = SummonSpreadAngle_Deg / FMath::Max(1, NumMinions - NumMinions % 2);
		int32 RealStep = 0;
		for (int32 Step = 0; Step < NumMinions; Step++)
		{
			const FVector StepForward = AvatarForward.RotateAngleAxis(StepAngle_Deg * RealStep, FVector::UpVector);
			const float SummonDist_Cm = FMath::RandRange(MinSummonDistance_Cm, MaxSummonDistance_Cm);

			const FVector SummonLocation = AvatarLocation + StepForward * SummonDist_Cm;
			Result.Emplace(SummonLocation);

			RealStep *= -1;
			if (RealStep >= 0)
			{
				RealStep++;
			}
		}
	}

	if (UWorld* const World = GetWorld(); IsValid(World))
	{
		if (const UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(World); IsValid(NavSys))
		{
			for (FVector& SummonLocation : Result)
			{
				if (FNavLocation NavLocation;
					NavSys->ProjectPointToNavigation(SummonLocation, NavLocation, FVector{100.0f})
				)
				{
					SummonLocation = NavLocation.Location;
				}
			}
		}
	}

	return Result;
}

TSubclassOf<APawn> UAuraSummonGameplayAbility::ChooseSummonMinionClass() const
{
	if (MinionClasses.IsEmpty())
	{
		return nullptr;
	}

	const int32 ClassIdx = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[ClassIdx];
}
