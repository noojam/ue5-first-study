#include "UI/InventoryWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "UI/InventorySlotWidget.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventoryGrid || !InventorySlotClass)
	{
		return;
	}

	for (int32 i = 0; i < 20; i++)
	{
		UInventorySlotWidget* SlotWidget =
			CreateWidget<UInventorySlotWidget>(
				GetWorld(),
				InventorySlotClass);

		if (!SlotWidget)
		{
			continue;
		}

        SlotWidgets.Add(SlotWidget);

		UUniformGridSlot* GridSlot =
			InventoryGrid->AddChildToUniformGrid(
				SlotWidget,
				i / 4,
				i % 4);

		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
}

void UInventoryWidget::SetInventoryComponent(
    UInventoryComponent* InInventory)
{
    InventoryComponent = InInventory;
}

void UInventoryWidget::RefreshInventory()
{
    if (!InventoryComponent)
    {
        return;
    }

    const TArray<FInventorySlot>& Slots =
        InventoryComponent->GetSlots();

    for (int32 i = 0; i < SlotWidgets.Num(); i++)
    {
        if (i < Slots.Num())
        {
            SlotWidgets[i]->UpdateSlot(Slots[i]);
        }
        else
        {
            SlotWidgets[i]->UpdateSlot(FInventorySlot());
        }
    }
}
