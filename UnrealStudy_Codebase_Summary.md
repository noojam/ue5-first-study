# UnrealStudy 코드 구조 정리

이 문서는 **현재 `UnrealStudy` 프로젝트의 C++ 스크립트와 주요 에셋 구조를 다른 AI 에이전트에게 넘기기 위한 정리본**이다.

주의:
- 이 정리는 현재 워크스페이스에서 확인 가능한 C++ 코드와 파일 이름을 기준으로 작성했다.
- `.uasset` 같은 블루프린트/에셋 내부는 바이너리라서 직접 내용을 읽을 수 없기 때문에, C++에서 참조되는 구조와 파일명 기준으로만 정리했다.
- 현재 코드 스냅샷에는 TAB 입력 처리 관련해서 `MyCharacter` 쪽 바인딩과 `InventoryWidget` 쪽 키 처리 로직이 함께 남아 있다. 이 부분은 구조 이해용으로 함께 적는다.

## 1. 프로젝트 전체 구조

이 프로젝트는 학습 목적의 Unreal Engine C++ + Blueprint 혼합 구조다.

핵심 흐름은 다음과 같다.

- `AMyPlayerController`
  - Enhanced Input Mapping Context를 등록하는 진입점.
- `AMyCharacter`
  - 플레이어 캐릭터의 실제 입력 처리, 이동, 점프, 스프린트, 상호작용, 인벤토리 UI 생성 담당.
- `UInteractionComponent`
  - 월드에서 상호작용 가능한 대상을 Sphere Trace로 찾고, 인터페이스를 통해 상호작용 요청을 전달.
- `IInteractableInterface`
  - 상호작용 가능한 객체들이 공통으로 구현하는 규약.
- `ADoorActor`, `AItemActor`
  - 인터페이스 구현 예시.
- `UInventoryComponent`
  - 슬롯 기반 인벤토리 데이터 보관.
- `UInventoryWidget`, `UInventorySlotWidget`
  - 인벤토리 UI를 동적으로 생성하고 갱신하는 UMG 위젯.
- `FItemData`, `FInventorySlot`
  - 아이템 고정 정보와 슬롯 상태를 저장하는 구조체.
- `AMyPaddle`, `ARotatingActor`
  - 메인 TPS 구조와 분리된 실습용 예제 코드.

---

## 2. 파일별 정리

## 2-1. `Source/UnrealStudy/UnrealStudy.h`

역할:
- 프로젝트 모듈 공용 헤더.
- 현재 내용은 거의 비어 있고 `CoreMinimal.h`만 포함한다.

구조:
- `#pragma once`
- `#include "CoreMinimal.h"`

의미:
- 모듈 공통 include 용도.
- 실제 게임플레이 로직은 없다.

## 2-2. `Source/UnrealStudy/UnrealStudy.cpp`

역할:
- 메인 게임 모듈 정의.

구조:
- `IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, UnrealStudy, "UnrealStudy");`

의미:
- Unreal 엔진이 이 프로젝트를 게임 모듈로 인식하게 하는 진입점.
- 학습용으로는 거의 건드릴 일이 없다.

---

## 2-3. `Source/UnrealStudy/Public/MyPlayerController.h`

역할:
- 플레이어 입력 매핑 컨텍스트를 등록하는 컨트롤러.

핵심 멤버:
- `UInputMappingContext* DefaultIMC`

핵심 함수:
- `BeginPlay()`

의미:
- 입력 자체를 캐릭터에 직접 넣는 게 아니라, 컨트롤러가 먼저 입력 규칙 세트를 등록한다.
- Unreal에서 입력 시스템을 어디에서 초기화하는지 이해하기 좋은 예시다.

## 2-4. `Source/UnrealStudy/Private/MyPlayerController.cpp`

현재 구조:
- `BeginPlay()`에서 `GetLocalPlayer()`를 얻는다.
- `UEnhancedInputLocalPlayerSubsystem`을 찾아서 `DefaultIMC`를 `AddMappingContext`로 등록한다.

흐름:
1. 로컬 플레이어 확인
2. Enhanced Input LocalPlayer Subsystem 획득
3. Mapping Context 등록

의미:
- `IMC_Default` 같은 입력 규칙 세트를 실제 플레이어에게 붙이는 단계.
- 이 프로젝트의 입력 시작점이다.

학습 포인트:
- `AddMappingContext`는 "이 플레이어가 어떤 입력 규칙을 사용할지 등록하는 것"이다.
- `BindAction`과는 역할이 다르다. `AddMappingContext`는 입력 규칙 연결, `BindAction`은 그 규칙이 발동했을 때 무엇을 할지 연결이다.

---

## 2-5. `Source/UnrealStudy/Public/MyCharacter.h`

역할:
- 실제 플레이어 캐릭터 클래스.
- 캐릭터 구조, 입력 액션, 상호작용, 인벤토리 UI 참조를 한 곳에서 관리한다.

상속:
- `ACharacter`

주요 컴포넌트:
- `USpringArmComponent* SpringArmComp`
- `UCameraComponent* CameraComp`
- `UInteractionComponent* InteractionComponent`
- `UInventoryComponent* InventoryComponent`

주요 입력 액션 포인터:
- `UInputAction* MoveAction`
- `UInputAction* LookAction`
- `UInputAction* JumpAction`
- `UInputAction* SprintAction`
- `UInputAction* InteractAction`

현재 스냅샷 기준 추가 입력 관련 상태:
- `ToggleInventory()` 함수가 존재한다.
- `bInventoryOpen` 불리언이 있다.

속도/점프 관련 값:
- `WalkSpeed`
- `SprintSpeed`
- `JumpVelocity`

UI 관련:
- `TSubclassOf<UInventoryWidget> InventoryWidgetClass`
- `UInventoryWidget* InventoryWidget`

추가 함수:
- `Move`
- `Look`
- `StartJump`
- `StopJump`
- `StartSprint`
- `StopSprint`
- `Interact`
- `ToggleInventory`
- `RefreshInventoryWidget`

의미:
- 이 클래스는 단순 캐릭터가 아니라, 플레이어가 가진 기능을 한 번에 조립하는 메인 허브다.
- Unreal 관점에서 "Actor + 여러 Component + UI 참조"의 조합을 학습하기 좋다.

---

## 2-6. `Source/UnrealStudy/Private/MyCharacter.cpp`

현재 구조 요약:
- 생성자에서 SpringArm, Camera, InteractionComponent, InventoryComponent를 생성한다.
- BeginPlay에서 점프 세팅과 인벤토리 위젯 생성을 한다.
- SetupPlayerInputComponent에서 Enhanced Input 바인딩을 한다.
- 이동/시점/점프/스프린트/상호작용/인벤토리 토글을 처리한다.

### 생성자

생성 내용:
- `SpringArmComp = CreateDefaultSubobject<USpringArmComponent>()`
- `SpringArmComp->SetupAttachment(GetCapsuleComponent())`
- `CameraComp = CreateDefaultSubobject<UCameraComponent>()`
- `CameraComp->SetupAttachment(SpringArmComp)`
- `InteractionComponent = CreateDefaultSubobject<UInteractionComponent>()`
- `InventoryComponent = CreateDefaultSubobject<UInventoryComponent>()`

의미:
- 캐릭터가 스스로 카메라와 기능 컴포넌트를 가진다.
- Unity식으로 보면 "캐릭터에 여러 컴포넌트를 붙여 기능을 조립"하는 구조와 비슷하지만, Unreal은 이것을 C++ 생성자에서 명시적으로 구성한다.

### BeginPlay()

현재 동작:
- `JumpZVelocity`를 `JumpVelocity`로 세팅한다.
- `InventoryWidgetClass`가 지정되어 있으면 `CreateWidget`으로 위젯을 만든다.
- 생성되면 `AddToViewport()`를 호출한다.
- 위젯은 시작 시 `Collapsed` 상태로 숨긴다.
- `SetInventoryComponent(InventoryComponent)`로 데이터 연결을 해준다.
- `RefreshInventory()`를 한 번 호출한다.

의미:
- UI 생성은 캐릭터 시작 시점에 한다.
- 인벤토리 UI는 캐릭터가 소유하고, 그 캐릭터의 컴포넌트 데이터를 표시한다.

### Move()

현재 동작:
- `FVector2D` 입력을 받는다.
- `Controller->GetControlRotation()`의 Yaw를 기준으로 앞으로/오른쪽 방향 벡터를 계산한다.
- `AddMovementInput(ForwardDirection, MovementVector.X)`
- `AddMovementInput(RightDirection, MovementVector.Y)`

의미:
- 카메라 방향 기준 이동.
- 현재 코드상 X/Y 축 해석은 프로젝트에서 따로 맞춰둔 상태다.

### Look()

현재 동작:
- `AddControllerPitchInput(-LookAxisVector.Y)`
- `AddControllerYawInput(LookAxisVector.X)`

의미:
- 마우스/스틱 기반 카메라 회전 처리.

### StartJump() / StopJump()

현재 동작:
- `Jump()` / `StopJumping()` 호출.

의미:
- Unreal Character의 점프 시스템을 그대로 사용한다.
- 직접 중력/속도 계산을 하지 않는다.

### StartSprint() / StopSprint()

현재 동작:
- 공중 상태면 스프린트 시작을 막는다.
- 달릴 때 `MaxWalkSpeed = SprintSpeed`
- 멈출 때 `MaxWalkSpeed = WalkSpeed`

의미:
- 스프린트는 속도 값 변경으로 구현한다.
- 학습 포인트로는 속도 값을 하드코딩하지 않고 변수화한 점이 중요하다.

### Interact()

현재 동작:
- `InteractionComponent->TryInteract()` 호출.

의미:
- 실제 상호작용 판정은 컴포넌트가 담당하고, 캐릭터는 요청만 전달한다.

### ToggleInventory()

현재 스냅샷 기준:
- `bInventoryOpen`을 토글한다.
- `APlayerController`를 얻는다.
- 열 때:
  - `SetGamePaused(true)`
  - `SetInputMode(FInputModeGameAndUI())`
  - `InventoryWidget->SetVisibility(Visible)`
  - `RefreshInventoryWidget()` 호출
- 닫을 때:
  - `SetGamePaused(false)`
  - `SetInputMode(FInputModeGameOnly())`
  - `InventoryWidget->SetVisibility(Collapsed)`

의미:
- 인벤토리를 열면 게임 시간이 멈추고, 닫으면 다시 재생된다.
- UI와 게임 입력 모드를 분리해서 다루는 전형적인 구조다.

### RefreshInventoryWidget()

현재 동작:
- `InventoryWidget`이 있으면 `RefreshInventory()`를 호출한다.

의미:
- 데이터가 바뀐 뒤 UI를 다시 그리는 별도 단계.
- AddItem만으로는 화면이 자동 갱신되지 않으므로, 별도로 호출해야 한다.

### SetupPlayerInputComponent()

현재 바인딩:
- `MoveAction -> Move`
- `LookAction -> Look`
- `JumpAction Started -> StartJump`
- `JumpAction Completed -> StopJump`
- `SprintAction Triggered -> StartSprint`
- `SprintAction Completed -> StopSprint`
- `InteractAction Started -> Interact`
- `Tab -> ToggleInventory` (`BindKey` 사용)

중요 메모:
- 현재 코드 스냅샷에서는 TAB 토글이 Enhanced Input 액션이 아니라 `BindKey(EKeys::Tab, ...)`로도 연결되어 있다.
- 동시에 `InventoryWidget`에도 TAB 키 처리 로직이 남아 있어서, 입력 처리 구조가 조금 중복되어 있다.
- 학습 목적이라면 이후에 `IA_Inventory` 액션으로 통일하는 편이 더 정석이다.

---

## 2-7. `Source/UnrealStudy/Public/InteractionComponent.h`

역할:
- 상호작용 대상 탐색과 실행을 담당하는 Actor Component.

주요 함수:
- `TryInteract()`
- `TickComponent()`
- `GetCurrentInteractable()`
- `BeginPlay()`
- `UpdateInteractTarget()`

주요 값:
- `InteractionDistance = 30.f`
- `SphereRadius = 24.f`
- `CurrentInteractable`

의미:
- 캐릭터 본체에서 상호작용 로직을 분리한 구조.
- 추후 하이라이트, UI 표시, 포커스 상태 등으로 확장하기 좋다.

---

## 2-8. `Source/UnrealStudy/Private/InteractionComponent.cpp`

현재 구조:
- Tick마다 `UpdateInteractTarget()`를 호출한다.
- `GetOwner()`가 유효한지 확인한다.
- 소유자가 `ACharacter`인지 확인한다.
- 소유자에게 `UCameraComponent`가 있는지 확인한다.
- 현재 코드에서는 카메라 존재 여부만 확인하고, 실제 Sphere Trace 시작점은 `OwnerActor->GetActorLocation()`과 `GetActorForwardVector()`를 사용한다.
- `UKismetSystemLibrary::SphereTraceSingle()`로 전방을 짧게 검사한다.
- 맞은 Actor가 `IInteractableInterface`를 구현하면 `CurrentInteractable`에 저장한다.
- `TryInteract()`는 `Execute_Interact(CurrentInteractable, Cast<APawn>(OwnerActor))`를 호출한다.

학습 포인트:
- 입력을 직접 처리하지 않고, 기능을 독립된 컴포넌트로 분리했다.
- 인터페이스 호출은 `Execute_` 패턴을 통해 블루프린트/네이티브 공통으로 처리한다.

주의점:
- 현재 코드상 카메라 컴포넌트를 찾지만, trace 시작점은 카메라 위치가 아니라 오너 액터 위치다.
- 학습 단계에서는 현재 구조를 이해하되, 나중에 카메라 기준으로 바꾸면 더 자연스러운 상호작용이 된다.

---

## 2-9. `Source/UnrealStudy/Public/InteractableInterface.h`

역할:
- 상호작용 가능한 대상이 반드시 가져야 하는 계약.

구조:
- `UINTERFACE(MinimalAPI) class UInteractableInterface : public UInterface`
- `class IInteractableInterface`

함수:
- `Interact(APawn* InstigatorPawn)`
- `GetInteractText() const`

특징:
- 둘 다 `BlueprintNativeEvent`.
- 실제 구현은 `_Implementation` 함수에 들어간다.

학습 포인트:
- `Interact`는 선언 이름.
- `Interact_Implementation`은 실제 구현 함수.
- `Execute_Interact()`는 UHT가 제공하는 디스패치 호출 방식.

의미:
- 문, 아이템, 기타 오브젝트가 공통 인터랙션 규약을 공유할 수 있다.

---

## 2-10. `Source/UnrealStudy/Public/DoorActor.h`

역할:
- 상호작용 가능한 문 예제.

상속:
- `AActor`
- `IInteractableInterface`

주요 멤버:
- `UStaticMeshComponent* DoorMesh`
- `bool bIsOpen`
- `float OpenAngle`
- `float OpenDuration`
- `ReceiveToggleDoor(bool bNewOpenState)`

의미:
- C++에서 상태를 바꾸고, 블루프린트에서 연출을 덮어쓸 수 있는 구조다.
- 학습용으로 "코드와 BP의 역할 분리"를 보기 좋다.

---

## 2-11. `Source/UnrealStudy/Private/DoorActor.cpp`

현재 구조:
- 생성자에서 `DoorMesh`를 만들고 루트로 지정한다.
- Tick은 꺼져 있다.
- `Interact_Implementation()`에서 `bIsOpen`을 반전시킨다.
- `ReceiveToggleDoor(bIsOpen)`를 호출해 BP 연출을 넘긴다.
- `GetInteractText_Implementation()`은 열려 있으면 `Close Door`, 닫혀 있으면 `Open Door`를 반환한다.

의미:
- 문 상태는 C++가 관리하고, 실제 애니메이션/연출은 BP에서 붙이기 쉬운 구조다.

---

## 2-12. `Source/UnrealStudy/Public/ItemData.h`

역할:
- 아이템 고정 정보와 인벤토리 슬롯 상태를 저장하는 구조체 정의.

### `FItemData`

필드:
- `FName ItemID`
- `FText ItemName`
- `int32 MaxStackSize = 10`

의미:
- 아이템의 정체성과 스택 제한을 담는 데이터.
- 현재 학습 단계에서는 DataAsset/DataTable 대신 구조체 직참조 방식으로도 충분히 쓸 수 있다.

### `FInventorySlot`

필드:
- `FItemData ItemData`
- `int32 Count = 0`

함수:
- `IsEmpty() const`
- `Clear()`

의미:
- 한 슬롯이 어떤 아이템을 몇 개 들고 있는지 표현한다.
- `Count <= 0`이면 빈 슬롯으로 본다.

학습 포인트:
- `FItemData`는 "아이템의 정적 정보".
- `FInventorySlot`은 "실제 인벤토리 상태".
- 둘을 분리한 이유는 동일 아이템을 여러 개 쌓아 관리하기 쉽기 때문이다.

---

## 2-13. `Source/UnrealStudy/Public/InventoryComponent.h`

역할:
- 슬롯 배열을 가진 인벤토리 데이터 컴포넌트.

주요 멤버:
- `TArray<FInventorySlot> Slots`
- `int32 MaxInventorySize = 20`

주요 함수:
- `AddItem(const FItemData& NewItem, int32 Count = 1)`
- `GetSlots() const`
- `BeginPlay()`

의미:
- 인벤토리의 실제 데이터 저장소.
- UI는 이 컴포넌트에서 데이터를 읽어 그리기만 한다.

---

## 2-14. `Source/UnrealStudy/Private/InventoryComponent.cpp`

현재 구조:
- 생성자에서 Tick을 켠다.
- BeginPlay에서 `Slots.SetNum(MaxInventorySize)`로 20칸을 만든다.
- `AddItem()`은 먼저 기존 스택 슬롯을 채우고, 그다음 빈 슬롯에 새로 넣는다.
- 아이템이 성공적으로 들어가면 `GEngine->AddOnScreenDebugMessage`로 성공 메시지를 출력한다.
- 공간이 없으면 실패 메시지를 출력한다.

세부 흐름:
1. `Count <= 0`이면 실패.
2. 같은 `ItemID`이고 스택이 남아 있는 슬롯을 먼저 찾는다.
3. 남은 수량이 있으면 빈 슬롯에 새 스택으로 넣는다.
4. 전부 못 넣으면 인벤토리 가득 참으로 처리한다.

학습 포인트:
- 같은 아이템 여러 개를 쌓는 구조를 구현한 상태다.
- UI는 자동 갱신하지 않는다. 별도 Refresh가 필요하다.

주의점:
- 현재 `AddItem()`은 수량 인자를 받을 수 있지만, 호출부는 아직 기본값만 쓰는 경우가 있다.
- 예를 들어 `AItemActor`는 `ItemCount`를 가지고 있어도 현재는 `AddItem(ItemData)`만 호출하는 구조다.

---

## 2-15. `Source/UnrealStudy/Public/InventoryWidget.h`

역할:
- 전체 인벤토리 UI 위젯.

주요 멤버:
- `UUniformGridPanel* InventoryGrid` (`BindWidget`)
- `TSubclassOf<UUserWidget> InventorySlotClass`
- `TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets`
- `UInventoryComponent* InventoryComponent`
- `AMyCharacter* OwningCharacter` (현재 스냅샷에 남아 있음)

주요 함수:
- `SetInventoryComponent()`
- `SetOwningCharacter()`
- `RefreshInventory()`
- `NativeConstruct()`
- `NativeOnKeyDown()`

의미:
- 슬롯을 20개 동적으로 만들고, 인벤토리 데이터를 화면에 표시하는 역할.

중요 포인트:
- `InventoryGrid`는 디자이너에서 이름이 정확히 같아야 `BindWidget`이 된다.
- `InventorySlotClass`는 BP 슬롯 위젯 클래스가 들어가야 한다.

---

## 2-16. `Source/UnrealStudy/Private/InventoryWidget.cpp`

현재 구조:
- `NativeConstruct()`에서 `InventoryGrid`와 `InventorySlotClass`를 검사한다.
- 20개의 `UInventorySlotWidget`을 `CreateWidget`으로 만든다.
- `InventoryGrid->AddChildToUniformGrid()`로 4x5 배열에 배치한다.
- 각 슬롯의 Horizontal/Vertical Alignment를 `Fill`로 둔다.
- `SetInventoryComponent()`는 데이터를 연결한다.
- `RefreshInventory()`는 슬롯 위젯 배열과 인벤토리 슬롯 배열을 1:1로 맞추어 `UpdateSlot()`을 호출한다.
- 현재 스냅샷에는 `SetOwningCharacter()`와 `NativeOnKeyDown()`도 남아 있다.
- `NativeOnKeyDown()`은 Tab을 누르면 `OwningCharacter->ToggleInventory()`를 호출한다.

학습 포인트:
- 위젯은 생성과 데이터 갱신이 분리되어 있다.
- `NativeConstruct()`는 위젯이 실제 생성된 뒤 한 번 실행되는 초기화 지점이다.

중요 메모:
- 현재 스냅샷에서는 TAB 토글 입력이 캐릭터 쪽 `BindKey`와 위젯 쪽 `NativeOnKeyDown` 양쪽에 남아 있다.
- 학습/정리 관점에서는 둘 중 하나로 통일하는 편이 더 낫다.

---

## 2-17. `Source/UnrealStudy/Public/InventorySlotWidget.h`

역할:
- 인벤토리 한 칸을 담당하는 슬롯 UI.

주요 멤버:
- `UTextBlock* ItemName` (`BindWidget`)
- `UTextBlock* ItemCount` (`BindWidget`)

주요 함수:
- `UpdateSlot(const FInventorySlot& SlotData)`

의미:
- 한 칸에 아이템 이름과 수량을 보여주는 단위 UI.
- 전체 인벤토리 UI는 이 위젯 20개를 조립한 구조다.

---

## 2-18. `Source/UnrealStudy/Private/InventorySlotWidget.cpp`

현재 구조:
- `UpdateSlot()`에서 `ItemName` 또는 `ItemCount`가 null이면 바로 반환한다.
- `SlotData.IsEmpty()`이면 `Empty`와 `0`을 표시한다.
- 비어 있지 않으면 `SlotData.ItemData.ItemName`과 `SlotData.Count`를 표시한다.

의미:
- 이 함수는 "슬롯 데이터 -> 화면 텍스트" 변환만 담당한다.
- 비어 있는 슬롯도 아예 텍스트를 제거하지 않고 디버그용 문자를 넣는 상태다.

학습 포인트:
- `UpdateSlot()`은 UI 표시용 함수이지, 인벤토리 로직을 처리하는 함수가 아니다.
- `BindWidget`이 안 맞으면 이 함수는 조용히 아무것도 하지 않는다.

---

## 2-19. `Source/UnrealStudy/Public/ItemActor.h`

역할:
- 월드에 떨어져 있는 아이템 오브젝트.

상속:
- `AActor`
- `IInteractableInterface`

주요 멤버:
- `UStaticMeshComponent* ItemMesh`
- `FItemData ItemData`
- `int32 ItemCount = 1`

주요 함수:
- `Interact_Implementation(APawn* InteractingPawn)`
- `GetInteractText_Implementation() const`

의미:
- 플레이어가 상호작용하면 인벤토리에 들어가야 하는 아이템.
- 현재는 ItemActor 자체가 아이템 데이터를 들고 있다.

중요 메모:
- `ItemCount` 필드는 있지만, 현재 구현에서는 `AddItem(ItemData)`만 호출한다.
- 즉 현재 스냅샷에서는 실제로 1개만 넣는 구조다.
- 수량 기반 습득을 하려면 `AddItem(ItemData, ItemCount)`로 넘겨야 한다.

---

## 2-20. `Source/UnrealStudy/Private/ItemActor.cpp`

현재 구조:
- 생성자에서 `ItemMesh`를 만들고 루트로 둔다.
- `Interact_Implementation()`에서 `InteractingPawn`의 `UInventoryComponent`를 찾는다.
- `AddItem(ItemData)`가 성공하면 `Destroy()`한다.
- `GetInteractText_Implementation()`은 `Pick Up <아이템명>`을 반환한다.

의미:
- 아이템 습득의 최종 진입점.
- 인터랙션 시스템과 인벤토리 시스템을 연결하는 브리지 역할이다.

학습 포인트:
- Actor가 직접 인벤토리를 변경하는 것이 아니라, Pawn의 InventoryComponent를 찾아 전달한다.
- 성공 시 자기 자신을 제거하는 전형적인 월드 아이템 패턴이다.

---

## 2-21. `Source/UnrealStudy/Public/MyPaddle.h`

역할:
- 메인 TPS 구조와 별개로 만들어진 실습용 Pawn.

주요 멤버:
- `UInputMappingContext* DefaultContext`
- `UInputAction* MoveAction`
- `float MoveSpeed = 10.0f`
- `USpringArmComponent* SpringArmComp`
- `UCameraComponent* CameraComp`
- `UStaticMeshComponent* PaddleMesh`

의미:
- 입력/카메라/이동을 더 단순한 샘플에서 실험하기 위한 코드.
- 학습 과정에서 직접 움직이는 오브젝트 구조를 확인하기 좋다.

---

## 2-22. `Source/UnrealStudy/Private/MyPaddle.cpp`

현재 구조:
- 생성자에서 DummyRoot를 만들고, PaddleMesh / SpringArm / Camera를 연결한다.
- `SetupPlayerInputComponent()`에서 Enhanced Input Subsystem에 Mapping Context를 추가한다.
- `CastChecked<UEnhancedInputComponent>`를 사용해 MoveAction을 바인딩한다.
- `Move()`는 `SetActorLocation()`으로 직접 위치를 옮긴다.

특징:
- `AMyCharacter`와 달리 `CharacterMovementComponent`를 쓰지 않는다.
- 이동을 직접 좌표 변화로 처리하는 아주 단순한 실습용 구조다.

학습 포인트:
- `CastChecked`는 실패 시 크래시/assert 성격이 강하다.
- 학습용으로는 좋지만, 게임플레이 코드에서는 실패 가능성이 있으면 `Cast`가 더 안전할 때가 많다.

---

## 2-23. `Source/UnrealStudy/Public/RotatingActor.h`

역할:
- 회전 예제 Actor.

주요 멤버:
- `FRotator RotationSpeed`

의미:
- 매 프레임 회전하는 아주 단순한 샘플.
- Tick과 DeltaTime의 개념을 확인하기 좋다.

---

## 2-24. `Source/UnrealStudy/Private/RotatingActor.cpp`

현재 구조:
- 생성자에서 Tick을 켠다.
- Tick에서 `AddActorLocalRotation(RotationSpeed * DeltaTime)`를 호출한다.

의미:
- 프레임당 회전을 DeltaTime으로 보정하는 기본 예제.

---

## 3. 현재 콘텐츠 에셋 구조

### `Content/Inputs`

확인된 에셋:
- `IMC_Default`
- `IA_Move`
- `IA_Look`
- `IA_Jump`
- `IA_Sprint`
- `IA_Interaction`
- `BP_MyPlayerController`

의미:
- 입력 시스템의 중심 자산들이다.
- 현재 프로젝트는 Enhanced Input을 사용한다.
- `MyPlayerController`에서 `IMC_Default`를 등록하고, 캐릭터에서 액션을 바인딩한다.

### `Content/UI`

확인된 에셋:
- `WBP_Inventory`
- `WBP_InventorySlot`
- `WBP_InteractionGuide`

의미:
- `WBP_Inventory`는 전체 인벤토리 UI.
- `WBP_InventorySlot`은 한 칸 슬롯 UI.
- `WBP_InteractionGuide`는 상호작용 안내용 UI로 보인다.

### `Content/Levels`

확인된 맵:
- `TestLevel.umap`

의미:
- 현재 학습 흐름을 시험하는 테스트 맵으로 보인다.

---

## 4. 현재 코드 흐름 한 줄 요약

1. `AMyPlayerController`가 `IMC_Default`를 등록한다.
2. `AMyCharacter`가 입력을 바인딩하고, 카메라/상호작용/인벤토리 컴포넌트를 소유한다.
3. `UInteractionComponent`가 전방 Sphere Trace로 인터랙션 대상을 찾는다.
4. `AItemActor`와 `ADoorActor`는 `IInteractableInterface`를 구현해서 서로 다른 행동을 한다.
5. `UInventoryComponent`가 슬롯 배열에 아이템을 쌓는다.
6. `UInventoryWidget`가 20칸 슬롯 UI를 만들고 갱신한다.
7. `UInventorySlotWidget`가 한 칸의 텍스트를 표시한다.

---

## 5. 이 프로젝트에서 특히 중요한 학습 포인트

- Unreal은 "빈 캔버스에 스크립트 하나"가 아니라, **Actor / Component / Interface / Widget / Asset을 조합하는 구조**다.
- C++는 구조와 규칙을 잡고, Blueprint는 데이터와 연출을 붙이는 쪽이다.
- 입력은 `Mapping Context`와 `BindAction`을 분리해서 이해해야 한다.
- 상호작용은 `Interface` + `Trace` + `Execute_` 패턴이 핵심이다.
- 인벤토리는 `Data(FItemData)`와 `State(FInventorySlot)`를 분리해야 확장하기 쉽다.
- UI는 데이터 변경만으로 자동 갱신되지 않으므로, `RefreshInventory()` 같은 갱신 단계를 별도로 둬야 한다.

---

## 6. 다른 AI 에이전트에게 넘길 때 우선 읽을 순서

추천 순서:
1. `MyPlayerController.h / .cpp`
2. `MyCharacter.h / .cpp`
3. `InteractionComponent.h / .cpp`
4. `InteractableInterface.h`
5. `ItemData.h`
6. `InventoryComponent.h / .cpp`
7. `InventoryWidget.h / .cpp`
8. `InventorySlotWidget.h / .cpp`
9. `ItemActor.h / .cpp`
10. `DoorActor.h / .cpp`
11. `MyPaddle.h / .cpp`
12. `RotatingActor.h / .cpp`

---

## 7. 현재 스냅샷의 주의점

- TAB 인벤토리 토글 로직이 현재는 약간 중복된 상태로 남아 있다.
- `ItemActor`는 `ItemCount`를 가지고 있지만 현재 습득 호출에는 1개만 전달되는 구조다.
- `InteractionComponent`는 카메라 존재 여부를 확인하지만 trace 시작점은 아직 카메라 위치가 아니라 오너 액터 위치다.
- 슬롯 UI는 `BindWidget` 이름이 정확히 맞아야 한다.

이 문서는 이후 세션에서 구조를 빠르게 복기하기 위한 기준 문서로 사용하면 된다.
