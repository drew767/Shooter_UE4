// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputControllers/IInputController.h"
#include "KeyboardInputController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UKeyboardInputController : public UIInputController
{
	GENERATED_BODY()

public:
	UKeyboardInputController();
	void Tick(float deltaTime);

protected:
	virtual void RotationYawInput(float value) override;
	virtual	void RotationPitchInput(float value) override;
	virtual EInputDevice GetSupportedDevice() override;
};
