

#include "Components/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Slots.SetNum(MaxInventorySize);
	
}

bool UInventoryComponent::AddItem(const FItemData& NewItem, int32 Count)
{
	if(Count <= 0) return false;

    int32 Remaining = Count;

	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}

		if (Slot.ItemData.ItemID == NewItem.ItemID && Slot.Count < Slot.ItemData.MaxStackSize)
		{
			const int32 Space = Slot.ItemData.MaxStackSize - Slot.Count;
			const int32 ToAdd = FMath::Min(Space, Remaining);

			Slot.Count += ToAdd;
			Remaining -= ToAdd;

			if (Remaining <= 0)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						5.0f,
						FColor::Blue,
						FString::Printf(TEXT("Added Item: %s"), *NewItem.ItemName.ToString())
					);
				}

				return true;
			}
		}
	}

	for (FInventorySlot& Slot : Slots)
	{
		if (!Slot.IsEmpty())
		{
			continue;
		}

		const int32 ToAdd = FMath::Min(NewItem.MaxStackSize, Remaining);

		Slot.ItemData = NewItem;
		Slot.Count = ToAdd;
		Remaining -= ToAdd;

		if (Remaining <= 0)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Blue,
					FString::Printf(TEXT("Added Item: %s"), *NewItem.ItemName.ToString())
				);
			}

			return true;
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Red,
			FString::Printf(TEXT("Inventory Full: %s"), *NewItem.ItemName.ToString())
		);
	}

	return false;
}



