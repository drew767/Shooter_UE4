// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <InputState.h>
#include "CIngameCharacterPlayerController.h"
#include <Tickable.h>
#include <Stats/Stats2.h>
#include "IInputController.generated.h"

class ACCharacter;

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UIInputController : public UObject
{
	GENERATED_BODY()
public:
	UIInputController();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent, ACCharacter* character) final;
	virtual void Tick(float deltaTime);
	virtual EInputDevice GetSupportedDevice();
	void DetachInput(UInputComponent* PlayerInputComponent);

protected:
	virtual void Crouch();
	virtual void Jump();
	virtual void StartFire();
	virtual void StopFire();
	virtual void MoveForwardInput(float value);
	virtual void MoveRightInput(float value);
	virtual void ApplyMovementInput() {}

	virtual void RotationYawInput(float value);
	virtual	void RotationPitchInput(float value);
	virtual void ApplyRotationInput() {}
	
	virtual void OnStartTargeting();
	virtual void OnEndTargeting();
	virtual void OnReload();
	virtual void OnNextWeapon();
	virtual void OnPrevWeapon();
	virtual void OnEquipWeapon1();
	virtual void OnEquipWeapon2();
	virtual void OnEquipWeapon3();

	virtual void DeviceKeyPressed(FKey key);

	void RotateCharacterToLookAtRotation(float deltaTime);
	
protected:
	ACCharacter* m_character;
	ACIngameCharacterPlayerController* m_playerController;

	FVector m_movementDirectionInput;
	bool m_xMovementValid;
	bool m_yMovementValid;

	FVector m_rotationDirectionInput;
	bool m_xRotationValid;
	bool m_yRotationValid;

	FRotator m_lookAtRotation;
	
	bool m_inputIsBinded;
	bool m_acceptMovementAsRotationDirection;
	
	FName m_moveForwardSignal;
	FName m_moveRightSignal;
	FName m_lookXSignal;
	FName m_lookYSignal;
	FName m_crouchSignal;
	FName m_jumpSignal;
	FName m_fireSignal;
	FName m_detectDeviceSignal;

	FName m_aimingSignal;
	FName m_reloadSignal;
	FName m_nextWeaponSignal;
	FName m_previousWeaponSignal;

	FName m_weapon1Signal;
	FName m_weapon2Signal;
	FName m_weapon3Signal;
};
