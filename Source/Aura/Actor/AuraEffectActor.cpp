// Copyright Diegothic


#include "AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	SetRootComponent(TriggerSphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerSphere);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnTriggerBeginOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnTriggerEndOverlap);
}

void AAuraEffectActor::OnTriggerBeginOverlap_Implementation(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// TODO: Change this to apply a Gameplay Effect.
	// For now using a const_cast hack to modify the attributes.
	if (OtherActor->Implements<UAbilitySystemInterface>())
	{
		if (const IAbilitySystemInterface* const AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			if (
				UAuraAttributeSet* const AttributeSet = const_cast<UAuraAttributeSet*>(Cast<UAuraAttributeSet>(
					AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(
						UAuraAttributeSet::StaticClass())
				))
			)
			{
				AttributeSet->SetHealth(AttributeSet->GetHealth() + 25.0f);

				Destroy();
			}
		}
	}
}

void AAuraEffectActor::OnTriggerEndOverlap_Implementation(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
}
