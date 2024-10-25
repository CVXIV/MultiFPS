// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiFPSAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MultiFPS/Character/MultiFPSCharacter.h"

void UMultiFPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MultiFPSCharacter = Cast<AMultiFPSCharacter>(TryGetPawnOwner());
}

void UMultiFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!MultiFPSCharacter)
	{
		MultiFPSCharacter = Cast<AMultiFPSCharacter>(TryGetPawnOwner());
	}
	if (!MultiFPSCharacter)
	{
		return;
	}
	FVector Velocity = MultiFPSCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsInAir = MultiFPSCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = MultiFPSCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;
}
