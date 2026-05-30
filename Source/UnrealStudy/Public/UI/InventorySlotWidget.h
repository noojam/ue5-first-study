#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Datas/ItemData.h"

#include "InventorySlotWidget.generated.h"

class UTextBlock;

UCLASS()
class UNREALSTUDY_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void UpdateSlot(const FInventorySlot& SlotData);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;
};