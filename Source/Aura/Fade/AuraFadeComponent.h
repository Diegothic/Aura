// Copyright Diegothic

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AuraFadeComponent.generated.h"


USTRUCT()
struct FAuraFadeMeshEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<UMaterialInterface>> DefaultMaterials;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionResponse> CameraCollisionResponse;
};

UENUM()
enum class EAuraFadeState : uint8
{
	FadedIn,
	FadedOut,
	FadingOut,
	FadingIn,
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class AURA_API UAuraFadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAuraFadeComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	// ~ UActorComponent

	void FadeOut(UObject* InSource);
	void FadeIn(UObject* InSource);
	void CompleteFadeInstant();

private:
	void SetupEntries();
	void SetupDynamicMaterial();

	void RefreshFade();
	void FadeTimer();

	void UseDefaultMaterials();
	void UseDynamicMaterial();
	void SetDynamicMaterialFadeParameter(float InValue);

	void SetCollisionResponse(bool bInEnabled);

	UPROPERTY(EditDefaultsOnly, Category="Fade")
	TSoftObjectPtr<UMaterialInterface> FadeMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Fade", meta=(Units="Seconds"))
	float FadeDuration = 0.1f;

	UPROPERTY(Transient)
	TArray<FAuraFadeMeshEntry> FadeMeshEntries;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DynamicFadeMaterial;

	UPROPERTY(Transient)
	EAuraFadeState CurrentState = EAuraFadeState::FadedIn;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UObject>> FadeOutSources;

	float FadeOutPercent = 0.0f;
	FTimerHandle FadeTimerHandle;
	float LastUpdateTime = 0.0f;
};
