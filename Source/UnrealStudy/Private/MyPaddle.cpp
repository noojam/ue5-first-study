// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaddle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyPaddle::AMyPaddle()
{
	PrimaryActorTick.bCanEverTick = true;

    // 1. 최상위 루트가 될 빈 Scene 컴포넌트 생성 및 루트 지정
    USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DummyRoot;

    // 2. 큐브 메쉬 생성 및 루트에 부착 (형제 1)
    PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
    PaddleMesh->SetupAttachment(RootComponent); 

    // 3. 스프링 암 생성 및 루트에 부착 (형제 2)
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent); // 👈 큐브의 자식이 아니라 루트의 자식으로 대등하게!

    // 4. 카메라는 스프링 암의 '끝점(Socket)'에 완벽하게 종속되도록 자식으로 부착 (자식)
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    
    // ★ [핵심 부착 옵션] 스프링 암의 소켓 이름을 인자로 주어 자식으로 매답니다.
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AMyPaddle::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void AMyPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	    UE_LOG(LogTemp, Warning, TEXT("setupPlayerInputComponent Called"));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
		        UE_LOG(LogTemp, Warning, TEXT("Controller Valid"));

        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
			UE_LOG(LogTemp, Warning, TEXT("LocalPlayer Valid"));

            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
				 UE_LOG(LogTemp, Warning, TEXT("Subsystem Valid"));
                Subsystem->AddMappingContext(DefaultContext, 0);
            }
        }
    }

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPaddle::Move);
    }

}

void AMyPaddle::Move(const FInputActionValue& Value)
{
	 UE_LOG(LogTemp, Warning, TEXT("Move Called"));
    FVector2D MovementVector = Value.Get<FVector2D>();

    FVector NewLocation = GetActorLocation();

	NewLocation.X += MovementVector.X * MoveSpeed * GetWorld()->DeltaTimeSeconds;
NewLocation.Y += MovementVector.Y * MoveSpeed * GetWorld()->DeltaTimeSeconds;

	SetActorLocation(NewLocation);
}

