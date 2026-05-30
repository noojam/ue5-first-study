// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Interfaces/InteractableInterface.h"
#include "UI/InventoryWidget.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;

	if (InventoryWidgetClass)
	{
		InventoryWidget =
			CreateWidget<UInventoryWidget>(
				GetWorld(),
				InventoryWidgetClass);

		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

			InventoryWidget->SetInventoryComponent(
				InventoryComponent);

			InventoryWidget->RefreshInventory();
		}
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	 FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller == nullptr)
        return;

    FRotator ControlRotation =Controller->GetControlRotation();

    FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);

    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection,MovementVector.X);
    AddMovementInput(RightDirection,MovementVector.Y);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(-LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMyCharacter::StartJump(const FInputActionValue& Value)
{
	Jump();
}

void AMyCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void AMyCharacter::StartSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = GetSprintSpeed();
}

void AMyCharacter::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
}
void AMyCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract();
	}
}

void AMyCharacter::ToggleInventory()
{
	if (!InventoryWidget)
	{
		return;
	}

	bInventoryOpen = !bInventoryOpen;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = bInventoryOpen;

		if (bInventoryOpen)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			PlayerController->SetInputMode(FInputModeGameAndUI());
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			RefreshInventoryWidget();
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			PlayerController->SetInputMode(FInputModeGameOnly());
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AMyCharacter::RefreshInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->RefreshInventory();
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = 
	Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction
		(InventoryAction, ETriggerEvent::Started, this, &AMyCharacter::ToggleInventory);

		EnhancedInputComponent->BindAction
		(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		EnhancedInputComponent->BindAction
		(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		EnhancedInputComponent->BindAction
		(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::StartJump);

		EnhancedInputComponent->BindAction
		(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);

		EnhancedInputComponent->BindAction
		(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);

		EnhancedInputComponent->BindAction
		(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);

		EnhancedInputComponent->BindAction
		(InteractAction, ETriggerEvent::Started, this, &AMyCharacter::Interact);
	}

}

