// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <Components/DecalComponent.h>
#include "CIngameCharacterPlayerController.generated.h"

class ACCharacter;
class UDecalComponent;
class UIInputController;

UENUM(BlueprintType)
enum class EInputDevice : uint8
{
	None,
	KeyboardAndMouse,
	Gamepad
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDeviceChangedDelegate, EInputDevice, device);

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ACIngameCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACIngameCharacterPlayerController();

	void SetupInputForPlayer(ACCharacter* character);
	void DetachInputForPlayer();
	void SetCurrentInputDeviceType(EInputDevice device);
	virtual void Tick(float DeltaTime) override;
	EInputDevice GetCurrentInputDeviceType();

protected:
	void TickInputControllers(float DeltaTime);
	
	UFUNCTION()
		void OnDeviceChangedEvent(EInputDevice newDevice);

	FInputDeviceChangedDelegate m_OnDeviceChanged;

protected:
	FHitResult m_traceHitUnderCursorResult;
	EInputDevice m_currentInputDevice;

	UPROPERTY()
		TArray<UIInputController*> m_inputControllers;

};
