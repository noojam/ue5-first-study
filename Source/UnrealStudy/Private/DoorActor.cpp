#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"

ADoorActor::ADoorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    DoorMesh =
        CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));

    RootComponent = DoorMesh;
}

void ADoorActor::BeginPlay()
{
    Super::BeginPlay();
}

void ADoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADoorActor::Interact_Implementation(APawn* InstigatorPawn)
{
    bIsOpen = !bIsOpen;
    ReceiveToggleDoor(bIsOpen);
}

FText ADoorActor::GetInteractText_Implementation() const
{
    return bIsOpen ? FText::FromString("Close Door") : FText::FromString("Open Door");
}