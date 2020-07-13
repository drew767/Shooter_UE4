// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInputController.h"
#include "GamepadInputController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UGamepadInputController : public UIInputController
{
	GENERATED_BODY()
	
public:
	UGamepadInputController();
	void Tick(float deltaTime);

protected:
	virtual void RotationYawInput(float value) override;
    virtual	void RotationPitchInput(float value) override;

	virtual EInputDevice GetSupportedDevice() override;
	virtual void ApplyRotationInput() override;
	void MoveForwardInput(float value);
	void MoveRightInput(float value);
	virtual void ApplyMovementInput() override;
};
