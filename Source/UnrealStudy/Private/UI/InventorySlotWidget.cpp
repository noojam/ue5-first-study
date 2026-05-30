#include "UI/InventorySlotWidget.h"

#include "Components/TextBlock.h"

void UInventorySlotWidget::UpdateSlot(const FInventorySlot& SlotData)
{
	if (!ItemName || !ItemCount)
	{
		return;
	}

	if (SlotData.IsEmpty())
	{
		ItemName->SetText(FText::FromString(TEXT("Empty")));
		ItemCount->SetText(FText::FromString(TEXT("0")));
		return;
    }

	ItemName->SetText(SlotData.ItemData.ItemName);

	ItemCount->SetText(
		FText::AsNumber(SlotData.Count));
}