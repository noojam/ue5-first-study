// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractableInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALSTUDY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	void TryInteract();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentInteractable() const { return CurrentInteractable; }
	
	virtual void BeginPlay() override;
	
protected:
	void UpdateInteractTarget();

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 30.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float SphereRadius = 24.f;

	UPROPERTY()
	TObjectPtr<AActor> CurrentInteractable;
		
};
