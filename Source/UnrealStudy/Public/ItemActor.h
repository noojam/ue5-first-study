// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/InteractableInterface.h"
#include "Datas/ItemData.h"

#include "ItemActor.generated.h"

class UStaticMeshComponent;
class UInventoryComponent;

UCLASS()
class UNREALSTUDY_API AItemActor : public AActor, 
public IInteractableInterface
{
	GENERATED_BODY()
	
public:	

	AItemActor();

	virtual void Tick(float DeltaTime) override;


	virtual void Interact_Implementation(APawn* InteractingPawn) override;
	virtual FText GetInteractText_Implementation() const override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 ItemCount = 1;


};
