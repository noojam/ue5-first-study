// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MyPaddle.generated.h"

UCLASS()
class UNREALSTUDY_API AMyPaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* DefaultContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* MoveAction;


protected:
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere,Category="Camera Settings")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,Category="Camera Settings")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* PaddleMesh;

};
