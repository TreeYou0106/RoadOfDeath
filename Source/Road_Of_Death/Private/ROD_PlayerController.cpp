
#include "ROD_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ROD_Character.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AROD_PlayerController::AROD_PlayerController()
{
	// 显示鼠标
	bShowMouseCursor = true;
	// 初始化
	CachedDestination = FVector::ZeroVector;
}

void AROD_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	ROD_Character = CastChecked<AROD_Character>(GetPawn());
}

void AROD_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ROD_Character)
	{
		if (!ROD_Character->bIsDead)
		{
			TurnToCursor(DeltaTime);
			ROD_Character->Fire();
		}
	}
}

void AROD_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AROD_PlayerController::MoveForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AROD_PlayerController::MoveRight);

		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &AROD_PlayerController::FirePressed);
		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Completed, this, &AROD_PlayerController::FireReleased);
	}
}

void AROD_PlayerController::MoveForward(const FInputActionValue& Value)
{
	GetPawn()->AddMovementInput(FVector(1, 0, 0), Value.Get<float>() * ROD_Character -> MoveSpeed);
}

void AROD_PlayerController::MoveRight(const FInputActionValue& Value)
{
	GetPawn()->AddMovementInput(FVector(0, 1, 0), Value.Get<float>() * ROD_Character->MoveSpeed);
}

void AROD_PlayerController::TurnToCursor(float DeltaTime)
{
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->SetActorRotation(FRotator(0,WorldDirection.Rotation().Yaw, 0));
	}
}

void AROD_PlayerController::FirePressed()
{
	bIsFiringPressed = true;
}

void AROD_PlayerController::FireReleased()
{
	bIsFiringPressed = false;
}
