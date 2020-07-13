// Fill out your copyright notice in the Description page of Project Settings.


#include "InputControllers/IInputController.h"
#include "CCharacter.h"
#include "TopDownShooter/TopDownShooter.h"
#include <Components/InputComponent.h>
#include <Logging/LogMacros.h>

UIInputController::UIInputController() :
	m_xMovementValid(false),
	m_yMovementValid(false),
	m_xRotationValid(false),
	m_yRotationValid(false),
	m_inputIsBinded(false),
	m_acceptMovementAsRotationDirection(false)
{
	m_aimingSignal = "Aim";
	m_reloadSignal = "Reload";
	m_nextWeaponSignal = "NextWeapon";
	m_previousWeaponSignal = "PreviousWeapon";
	m_weapon1Signal = "Weapon1";
	m_weapon2Signal = "Weapon2";
	m_weapon3Signal = "Weapon3";

	m_crouchSignal = "Crouch";
	m_jumpSignal = "Jump";
	m_fireSignal = "Fire";

	m_detectDeviceSignal = "DetectDevice";
}

void UIInputController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent, ACCharacter* character)
{
	if (m_inputIsBinded)
	{
		return;
	}
	
	m_character = character;
	m_playerController = Cast<ACIngameCharacterPlayerController>(character->GetController());

	PlayerInputComponent->BindAxis(m_moveForwardSignal, this, &UIInputController::MoveForwardInput);
	PlayerInputComponent->BindAxis(m_moveRightSignal, this, &UIInputController::MoveRightInput);

	PlayerInputComponent->BindAxis(m_lookXSignal, this, &UIInputController::RotationYawInput);
	PlayerInputComponent->BindAxis(m_lookYSignal, this, &UIInputController::RotationPitchInput);

	PlayerInputComponent->BindAction(m_crouchSignal, IE_Pressed, this, &UIInputController::Crouch);
	PlayerInputComponent->BindAction(m_jumpSignal, IE_Pressed, this, &UIInputController::Jump);
	PlayerInputComponent->BindAction(m_fireSignal, IE_Pressed, this, &UIInputController::StartFire);
	PlayerInputComponent->BindAction(m_fireSignal, IE_Released, this, &UIInputController::StopFire);
	PlayerInputComponent->BindAction(m_aimingSignal, IE_Pressed, this, &UIInputController::OnStartTargeting);
	PlayerInputComponent->BindAction(m_aimingSignal, IE_Released, this, &UIInputController::OnEndTargeting);
	PlayerInputComponent->BindAction(m_reloadSignal, IE_Pressed, this, &UIInputController::OnReload);
	PlayerInputComponent->BindAction(m_nextWeaponSignal, IE_Pressed, this, &UIInputController::OnNextWeapon);
	PlayerInputComponent->BindAction(m_previousWeaponSignal, IE_Pressed, this, &UIInputController::OnPrevWeapon);
	PlayerInputComponent->BindAction(m_weapon1Signal, IE_Pressed, this, &UIInputController::OnEquipWeapon1);
	PlayerInputComponent->BindAction(m_weapon2Signal, IE_Pressed, this, &UIInputController::OnEquipWeapon2);
	PlayerInputComponent->BindAction(m_weapon3Signal, IE_Pressed, this, &UIInputController::OnEquipWeapon3);

	PlayerInputComponent->BindAction(m_detectDeviceSignal, IE_Pressed, this, &UIInputController::DeviceKeyPressed);
	m_inputIsBinded = true;
}

void UIInputController::OnStartTargeting()
{
	m_character->OnStartTargeting();
}
void UIInputController::OnEndTargeting()
{
	m_character->OnEndTargeting();
}
void UIInputController::OnReload()
{
}
void UIInputController::OnNextWeapon()
{
}
void UIInputController::OnPrevWeapon()
{
}
void UIInputController::OnEquipWeapon1()
{
}
void UIInputController::OnEquipWeapon2()
{
}
void UIInputController::OnEquipWeapon3()
{
}

void UIInputController::DeviceKeyPressed(FKey key)
{
	if (key.IsGamepadKey())
	{
		m_playerController->SetCurrentInputDeviceType(EInputDevice::Gamepad);
	} 
	else if (key.IsMouseButton() || !key.IsGamepadKey())
	{
		m_playerController->SetCurrentInputDeviceType(EInputDevice::KeyboardAndMouse);
	}
}

void UIInputController::Crouch()
{
	if (m_character->bIsCrouched)
	{
		m_character->UnCrouch();
	}
	else
	{
		m_character->Crouch();
	}
}

void UIInputController::Jump()
{
	m_character->Jump();
}

void UIInputController::StartFire()
{
	m_character->StartFire();
}

void UIInputController::StopFire()
{
	m_character->StopFire();
}

void UIInputController::MoveForwardInput(float value)
{
	m_character->AddMovementInput(m_character->GetActorForwardVector() * value);
}

void UIInputController::MoveRightInput(float value)
{
	m_character->AddMovementInput(m_character->GetActorRightVector() * value);
}

void UIInputController::RotationYawInput(float value)
{
	m_rotationDirectionInput.Y = value;
	m_yRotationValid = true;
}

void UIInputController::RotationPitchInput(float value)
{
	m_rotationDirectionInput.X = value;
	m_xRotationValid = true;
}

EInputDevice UIInputController::GetSupportedDevice()
{
	return EInputDevice::None;
}

void UIInputController::DetachInput(UInputComponent* PlayerInputComponent)
{
	// DIRTY HACK
	// SHOULD BE REPLACED WITH EVENT UNBINDING

	PlayerInputComponent->ClearActionBindings();
	PlayerInputComponent->VectorAxisBindings.Empty();
}

void UIInputController::Tick(float deltaTime)
{
	RotateCharacterToLookAtRotation(deltaTime);
}

void UIInputController::RotateCharacterToLookAtRotation(float deltaTime)
{
	//if (m_inputIsBinded && m_playerController->GetCurrentInputDeviceType() == GetSupportedDevice())
	//{
	//	FRotator characterRotation = FMath::RInterpTo(m_character->GetActorRotation(), m_lookAtRotation, deltaTime, 25);
	//	m_playerController->SetControlRotation(characterRotation);
	//}
}