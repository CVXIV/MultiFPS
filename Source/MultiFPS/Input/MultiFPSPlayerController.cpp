// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiFPSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

AMultiFPSPlayerController::AMultiFPSPlayerController() {
	bReplicates = true;
}

void AMultiFPSPlayerController::BeginPlay() {
	Super::BeginPlay();
	check(MappingContext)
	check(MoveAction)
	check(ViewAction)
	check(JumpAction)

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}

void AMultiFPSPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(ViewAction, ETriggerEvent::Triggered, this, &ThisClass::View);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
}

void AMultiFPSPlayerController::Move(const FInputActionValue& InputActionValue) {
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* Obj = GetPawn<APawn>()) {
		Obj->AddMovementInput(ForwardDirection, static_cast<float>(InputAxisVector.Y));
		Obj->AddMovementInput(RightDirection, static_cast<float>(InputAxisVector.X));
	}
}

void AMultiFPSPlayerController::View(const FInputActionValue& InputActionValue) {
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* Obj = GetPawn<APawn>()) {
		Obj->AddControllerYawInput(InputAxisVector.X);
		Obj->AddControllerPitchInput(InputAxisVector.Y);
	}
}

void AMultiFPSPlayerController::Jump(const FInputActionValue& InputActionValue) {
	if (InputActionValue.Get<bool>()) {
		if (ACharacter* Obj = GetPawn<ACharacter>()) {
			Obj->Jump();
		}
	}
}
