#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI/InventorySlotWidget.h"
#include "Components/InventoryComponent.h"

#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventoryComponent;

UCLASS()
class UNREALSTUDY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    void SetInventoryComponent(
    UInventoryComponent* InInventory);

    void RefreshInventory();

protected:

    // 위젯 생성 완료 후 실행
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> InventorySlotClass;

    UPROPERTY()
    TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;

    UPROPERTY()
    TObjectPtr<UInventoryComponent> InventoryComponent;


};