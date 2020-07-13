// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Delegates/DelegateCombinations.h>
#include "CRaidGameModeBase.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, victimActor, AActor*, killerActor, AController*, killerController);

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ACRaidGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACRaidGameModeBase();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	// Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewBot();

	void StartWave();
	void EndWave();

	void PrepareForNextWave();
	void SpawnBotTimerElapsed();
	void CheckAnyPlayerAlive();
	void GameOver();
	void RestartDeadPlayers();
	void CheckWaveState();
	void SetWaveState(EWaveState newState);

protected:
	FTimerHandle m_timerHandle_BotSpawner;
	FTimerHandle m_timerHandle_NextWaveStart;

	int32 m_numberOfBotsToSpawn;
	int32 m_waveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float m_timeBetweenWaves;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled m_onActorKilled;
};
