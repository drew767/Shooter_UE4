// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CRaidGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete,
	WaveComplete,
	GameOver
};

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ACRaidGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:

	UFUNCTION()
		void OnRep_WaveState(EWaveState oldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void WaveStateChanged(EWaveState newState, EWaveState oldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EWaveState m_waveState;

public:

	void SetWaveState(EWaveState newState);
};
