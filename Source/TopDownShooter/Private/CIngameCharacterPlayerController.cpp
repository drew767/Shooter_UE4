// Fill out your copyright notice in the Description page of Project Settings.


#include "CIngameCharacterPlayerController.h"

#include "InputControllers/KeyboardInputController.h"
#include "InputControllers/GamepadInputController.h"
#include "CCharacter.h"
#include <Materials/Material.h>
#include <UObject/ConstructorHelpers.h>
#include <Engine/Engine.h>
#include <GameFramework/Controller.h>
#include "IXRTrackingSystem.h"
#include "IXRCamera.h"
#include <DrawDebugHelpers.h>


ACIngameCharacterPlayerController::ACIngameCharacterPlayerController()
{
	m_inputControllers.Add(CreateDefaultSubobject<UGamepadInputController>(TEXT("GamepadInputController")));
	m_inputControllers.Add(CreateDefaultSubobject<UKeyboardInputController>(TEXT("KeyboardInputController")));
	m_OnDeviceChanged.AddDynamic(this, &ACIngameCharacterPlayerController::OnDeviceChangedEvent);
}

void ACIngameCharacterPlayerController::SetCurrentInputDeviceType(EInputDevice device)
{
	if (m_currentInputDevice != device)
	{
		m_currentInputDevice = device;
		m_OnDeviceChanged.Broadcast(m_currentInputDevice);
	}
}

void ACIngameCharacterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickInputControllers(DeltaTime);
}

void ACIngameCharacterPlayerController::TickInputControllers(float DeltaTime)
{
	for (auto* controller : m_inputControllers)
	{
		controller->Tick(DeltaTime);
	}
}

EInputDevice ACIngameCharacterPlayerController::GetCurrentInputDeviceType()
{
	return m_currentInputDevice;
}

void ACIngameCharacterPlayerController::SetupInputForPlayer(ACCharacter* character)
{
	for (auto* inputController : m_inputControllers)
	{
		if (inputController)
		{
			inputController->SetupPlayerInputComponent(InputComponent, character);
		}
	}
}

void ACIngameCharacterPlayerController::DetachInputForPlayer()
{
	for (auto* inputController : m_inputControllers)
	{
		inputController->DetachInput(InputComponent);
	}
}

void ACIngameCharacterPlayerController::OnDeviceChangedEvent(EInputDevice newDevice)
{
}