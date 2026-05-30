#include "Components/InteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Interfaces/InteractableInterface.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateInteractTarget();
}

void UInteractionComponent::UpdateInteractTarget()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		CurrentInteractable = nullptr;
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor);
	if (!OwnerCharacter)
	{
		CurrentInteractable = nullptr;
		return;
	}

	UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	if (!CameraComp)
	{
		CurrentInteractable = nullptr;
		return;
	}

	FVector Start = OwnerActor->GetActorLocation();

    FVector End = Start + OwnerActor->GetActorForwardVector() * InteractionDistance;

    FHitResult HitResult;

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerActor);

    bool bHit =
        UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            Start,
            End,
            SphereRadius,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            ActorsToIgnore,
            EDrawDebugTrace::None,
            HitResult,
            true
        );
	if (!bHit)
	{
		CurrentInteractable = nullptr;
        return;
	}
    
        AActor* HitActor = HitResult.GetActor();

	if (!HitActor)
	{
		CurrentInteractable = nullptr;
		return;
	}


	if(HitActor->Implements<UInteractableInterface>())
	{
		CurrentInteractable = HitActor;
	}
}

void UInteractionComponent::TryInteract()
{
	if (!CurrentInteractable)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	IInteractableInterface::Execute_Interact(CurrentInteractable, Cast<APawn>(OwnerActor));
}