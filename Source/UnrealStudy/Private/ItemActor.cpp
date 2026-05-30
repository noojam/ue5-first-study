// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"

#include "Components/InventoryComponent.h"
#include "UI/InventoryWidget.h"
#include "MyCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

}

void AItemActor::Interact_Implementation(APawn* InteractingPawn)
{
	if (!InteractingPawn) return;

	UInventoryComponent* Inventory = InteractingPawn->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		if (Inventory->AddItem(ItemData))
		{
			if (AMyCharacter* Character = Cast<AMyCharacter>(InteractingPawn))
			{
				Character->RefreshInventoryWidget();
			}

			Destroy();
		}
	}
}

FText AItemActor::GetInteractText_Implementation() const
{
	if (!ItemData.ItemName.IsEmpty())
	{
		return FText::FromString(FString::Printf(TEXT("Pick Up %s"), *ItemData.ItemName.ToString()));
	}

	return FText::FromString(TEXT("Pick Up Item"));
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

