// Copyright Diegothic


#include "AuraFadeComponent.h"


UAuraFadeComponent::UAuraFadeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAuraFadeComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupEntries();
	SetupDynamicMaterial();

	RefreshFade();
}

void UAuraFadeComponent::FadeOut(UObject* InSource)
{
	FadeOutSources.Emplace(InSource);

	RefreshFade();
}

void UAuraFadeComponent::FadeIn(UObject* InSource)
{
	FadeOutSources.Remove(InSource);

	RefreshFade();
}

void UAuraFadeComponent::CompleteFadeInstant()
{
	if (FadeTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}

	switch (CurrentState)
	{
	case EAuraFadeState::FadedIn:
	case EAuraFadeState::FadingIn:
		{
			CurrentState = EAuraFadeState::FadedIn;
			FadeOutPercent = 0.0f;
			UseDefaultMaterials();
			SetCollisionResponse(true);
			break;
		}
	case EAuraFadeState::FadedOut:
	case EAuraFadeState::FadingOut:
		{
			CurrentState = EAuraFadeState::FadedOut;
			FadeOutPercent = 1.0f;
			UseDynamicMaterial();
			SetDynamicMaterialFadeParameter(1.0f);
			SetCollisionResponse(true);
			break;
		}
	default:
		break;
	}
}

void UAuraFadeComponent::SetupEntries()
{
	FadeMeshEntries.Empty();

	const AActor* const Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}

	Owner->ForEachComponent<UMeshComponent>(false, [this](UMeshComponent* MeshComp)
	{
		if (!MeshComp->ComponentHasTag("Fade"))
		{
			return;
		}

		FAuraFadeMeshEntry Entry;
		Entry.MeshComponent = MeshComp;
		for (UMaterialInterface* const MaterialInst : MeshComp->GetMaterials())
		{
			Entry.DefaultMaterials.Emplace(MaterialInst);
		}
		Entry.CameraCollisionResponse = MeshComp->GetCollisionResponseToChannel(ECC_Camera);
		FadeMeshEntries.Emplace(Entry);
	});
}

void UAuraFadeComponent::SetupDynamicMaterial()
{
	if (UMaterialInterface* const FadeMaterialLoaded = FadeMaterial.LoadSynchronous())
	{
		DynamicFadeMaterial = UMaterialInstanceDynamic::Create(FadeMaterialLoaded, this);
	}
}

void UAuraFadeComponent::RefreshFade()
{
	bool bFireTimer = false;

	switch (CurrentState)
	{
	case EAuraFadeState::FadedIn:
	case EAuraFadeState::FadingIn:
		{
			if (!FadeOutSources.IsEmpty())
			{
				CurrentState = EAuraFadeState::FadingOut;
				bFireTimer = true;
			}

			break;
		}
	case EAuraFadeState::FadedOut:
	case EAuraFadeState::FadingOut:
		{
			if (FadeOutSources.IsEmpty())
			{
				CurrentState = EAuraFadeState::FadingIn;
				bFireTimer = true;
			}

			break;
		}
	default:
		break;
	}

	if (bFireTimer)
	{
		if (FadeTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		}

		LastUpdateTime = GetWorld()->GetTimeSeconds();
		FadeTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(
			this,
			&UAuraFadeComponent::FadeTimer
		);
	}
}

void UAuraFadeComponent::FadeTimer()
{
	const float FadeDuration_S = FMath::Max(FadeDuration, FLT_MIN);
	const float TimeDelta = GetWorld()->GetTimeSeconds() - LastUpdateTime;
	const float PercentDelta = TimeDelta / FadeDuration_S;
	LastUpdateTime = GetWorld()->GetTimeSeconds();

	bool bClearTimer = false;
	switch (CurrentState)
	{
	case EAuraFadeState::FadingIn:
		{
			FadeOutPercent = FMath::Clamp(FadeOutPercent - PercentDelta, 0.0f, 1.0f);
			SetDynamicMaterialFadeParameter(FadeOutPercent);

			if (FadeOutPercent <= 0.0f)
			{
				CurrentState = EAuraFadeState::FadedIn;
				bClearTimer = true;
				UseDefaultMaterials();
				SetCollisionResponse(true);
			}
			break;
		}
	case EAuraFadeState::FadingOut:
		{
			if (FadeOutPercent <= 0.0f)
			{
				UseDynamicMaterial();
				SetDynamicMaterialFadeParameter(0.0f);
				SetCollisionResponse(false);
			}

			FadeOutPercent = FMath::Clamp(FadeOutPercent + PercentDelta, 0.0f, 1.0f);
			SetDynamicMaterialFadeParameter(FadeOutPercent);

			if (FadeOutPercent >= 1.0f)
			{
				CurrentState = EAuraFadeState::FadedOut;
				bClearTimer = true;
				SetDynamicMaterialFadeParameter(1.0f);
			}
			break;
		}
	default:
		break;
	}

	if (bClearTimer)
	{
		if (FadeTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		}
	}
	else
	{
		FadeTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(
			this,
			&UAuraFadeComponent::FadeTimer
		);
	}
}

void UAuraFadeComponent::UseDefaultMaterials()
{
	for (const FAuraFadeMeshEntry& Entry : FadeMeshEntries)
	{
		for (int32 MaterialIdx = 0; MaterialIdx < Entry.DefaultMaterials.Num(); MaterialIdx++)
		{
			Entry.MeshComponent->SetMaterial(MaterialIdx, Entry.DefaultMaterials[MaterialIdx].Get());
		}
	}
}

void UAuraFadeComponent::UseDynamicMaterial()
{
	for (const FAuraFadeMeshEntry& Entry : FadeMeshEntries)
	{
		for (int32 MaterialIdx = 0; MaterialIdx < Entry.DefaultMaterials.Num(); MaterialIdx++)
		{
			Entry.MeshComponent->SetMaterial(MaterialIdx, DynamicFadeMaterial);
		}
	}
}

void UAuraFadeComponent::SetDynamicMaterialFadeParameter(const float InValue)
{
	if (!IsValid(DynamicFadeMaterial))
	{
		return;
	}

	DynamicFadeMaterial->SetScalarParameterValue("FadeOutPercent", InValue);
}

void UAuraFadeComponent::SetCollisionResponse(bool bInEnabled)
{
	for (const FAuraFadeMeshEntry& Entry : FadeMeshEntries)
	{
		for (int32 MaterialIdx = 0; MaterialIdx < Entry.DefaultMaterials.Num(); MaterialIdx++)
		{
			Entry.MeshComponent->SetCollisionResponseToChannel(
				ECC_Camera,
				bInEnabled
					? Entry.CameraCollisionResponse.GetValue()
					: ECR_Ignore
			);
		}
	}
}
