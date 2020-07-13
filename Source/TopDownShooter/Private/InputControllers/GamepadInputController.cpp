// Fill out your copyright notice in the Description page of Project Settings.


#include "InputControllers/GamepadInputController.h"
#include "CCharacter.h"
#include "CIngameCharacterPlayerController.h"
#include "TopDownShooter/TopDownShooter.h"

UGamepadInputController::UGamepadInputController()	
{
	m_moveForwardSignal = "MoveForwardGamepad";
	m_moveRightSignal = "MoveRightGamepad";
	m_lookXSignal = "LookGamepadX";
	m_lookYSignal = "LookGamepadY";
}

void UGamepadInputController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void UGamepadInputController::RotationYawInput(float value)
{
	Super::RotationYawInput(value);
	ApplyRotationInput();
}

void UGamepadInputController::RotationPitchInput(float value)
{
	Super::RotationPitchInput(value);
	ApplyRotationInput();
}

void UGamepadInputController::ApplyRotationInput()
{
	if (m_xRotationValid && m_yRotationValid && m_rotationDirectionInput.Size() > 0.1f)
	{
		if (m_rotationDirectionInput.Size() > 1.0f)
		{
			m_rotationDirectionInput.Normalize();
		}

		m_lookAtRotation = m_rotationDirectionInput.Rotation();
		m_acceptMovementAsRotationDirection = false;
	}
	else if (m_xRotationValid && m_yRotationValid)
	{
		m_xRotationValid = m_yRotationValid = false;
		m_acceptMovementAsRotationDirection = true;
	}
}

void UGamepadInputController::MoveForwardInput(float value)
{
	m_movementDirectionInput.X = value;
	m_xMovementValid = true;
	ApplyMovementInput();
}

void UGamepadInputController::MoveRightInput(float value)
{
	m_movementDirectionInput.Y = value;
	m_yMovementValid = true;
	ApplyMovementInput();
}

void UGamepadInputController::ApplyMovementInput()
{
	if (m_xMovementValid && m_yMovementValid && m_movementDirectionInput.Size() > 0.05f)
	{
		if (m_movementDirectionInput.Size() > 1.0f)
		{
			m_movementDirectionInput.Normalize();
		}

		m_character->AddMovementInput(m_movementDirectionInput);

		if (m_acceptMovementAsRotationDirection)
		{
			m_lookAtRotation = m_movementDirectionInput.Rotation();
		}

		m_movementDirectionInput = FVector::ZeroVector;
		m_xMovementValid = m_yMovementValid = false;
	}
	else if (m_xMovementValid && m_yMovementValid)
	{
		m_xMovementValid = m_yMovementValid = false;
		m_movementDirectionInput = FVector::ZeroVector;
	}
}

EInputDevice UGamepadInputController::GetSupportedDevice()
{
	return EInputDevice::Gamepad;
}
