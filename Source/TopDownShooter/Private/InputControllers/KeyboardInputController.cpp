// Fill out your copyright notice in the Description page of Project Settings.


#include "InputControllers/KeyboardInputController.h"
#include "CCharacter.h"
#include "CIngameCharacterPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "TopDownShooter/TopDownShooter.h"

UKeyboardInputController::UKeyboardInputController() 
{
	m_moveForwardSignal = "MoveForwardPC";
	m_moveRightSignal = "MoveRightPC";
	m_lookXSignal = "LookPCX";
	m_lookYSignal = "LookPCY";
}

void UKeyboardInputController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void UKeyboardInputController::RotationYawInput(float value)
{
	m_character->AddControllerYawInput(value);
}

void UKeyboardInputController::RotationPitchInput(float value)
{
	m_character->AddControllerPitchInput(value);
}

EInputDevice UKeyboardInputController::GetSupportedDevice()
{
	return EInputDevice::KeyboardAndMouse;
}