// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultiFPSAnimInstance.generated.h"

class AMultiFPSCharacter;

UCLASS()
class MULTIFPS_API UMultiFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Character)
	AMultiFPSCharacter* MultiFPSCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsAccelerating;
};
