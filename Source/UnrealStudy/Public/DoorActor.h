// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "DoorActor.generated.h"

UCLASS()
class UNREALSTUDY_API ADoorActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void ReceiveToggleDoor(bool bNewOpenState);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    bool bIsOpen = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Door")
    float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float OpenDuration = 1.0f;
};
