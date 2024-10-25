// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiFPSCharacter.generated.h"

UCLASS()
class MULTIFPS_API AMultiFPSCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AMultiFPSCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;
};
