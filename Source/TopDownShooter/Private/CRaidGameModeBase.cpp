// Fill out your copyright notice in the Description page of Project Settings.


#include "CRaidGameModeBase.h"
#include "CPlayerState.h"
#include "CRaidGameState.h"
#include "TimerManager.h"
#include "CHealthComponent.h"
#include <EngineUtils.h>
#include "CCharacter.h"

ACRaidGameModeBase::ACRaidGameModeBase() :
	m_timeBetweenWaves(2.0f)
{
	GameStateClass = ACRaidGameState::StaticClass();
	PlayerStateClass = ACPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ACRaidGameModeBase::StartPlay()
{
	Super::StartPlay();

	//PrepareForNextWave();
}

void ACRaidGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*CheckWaveState();
	CheckAnyPlayerAlive();*/
}

void ACRaidGameModeBase::StartWave()
{
	m_waveCount++;

	m_numberOfBotsToSpawn = 2 * m_waveCount;

	GetWorldTimerManager().SetTimer(m_timerHandle_BotSpawner, this, &ACRaidGameModeBase::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ACRaidGameModeBase::EndWave()
{
	GetWorldTimerManager().ClearTimer(m_timerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}

void ACRaidGameModeBase::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(m_timerHandle_NextWaveStart, this, &ACRaidGameModeBase::StartWave, m_timeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);
}

void ACRaidGameModeBase::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	m_numberOfBotsToSpawn--;

	if (m_numberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ACRaidGameModeBase::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator playerControllerIt = GetWorld()->GetPlayerControllerIterator(); playerControllerIt; ++playerControllerIt)
	{
		APlayerController* playerController = playerControllerIt->Get();
		if (playerController && playerController->GetPawn())
		{
			APawn* pawn = playerController->GetPawn();
			UCHealthComponent* healthComp = Cast<UCHealthComponent>(pawn->GetComponentByClass(UCHealthComponent::StaticClass()));
			if (ensure(healthComp) && healthComp->GetHealth() > 0.0f)
			{
				// A player is still alive.
				return;
			}
		}
	}

	// No player alive
	GameOver();
}


void ACRaidGameModeBase::GameOver()
{
	EndWave();

	//// @TODO: Finish up the match, present 'game over' to players.

	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("GAME OVER! Players Died"));
}


void ACRaidGameModeBase::SetWaveState(EWaveState newState)
{
	ACRaidGameState* gameState = GetGameState<ACRaidGameState>();
	if (ensureAlways(gameState))
	{
		gameState->SetWaveState(newState);
	}
}


void ACRaidGameModeBase::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator playerControllerIt = GetWorld()->GetPlayerControllerIterator(); playerControllerIt; ++playerControllerIt)
	{
		APlayerController* playerController = playerControllerIt->Get();
		if (playerController && playerController->GetPawn() == nullptr)
		{
			RestartPlayer(playerController);
		}
	}
}

void ACRaidGameModeBase::CheckWaveState()
{
	bool isPreparingForWave = GetWorldTimerManager().IsTimerActive(m_timerHandle_NextWaveStart);

	if (m_numberOfBotsToSpawn > 0 || isPreparingForWave)
	{
		return;
	}

	bool isAnyBotAlive = false;

	for (TActorIterator<APawn> actorItr(GetWorld()); actorItr; ++actorItr)
	{
		APawn* pawn = *actorItr;
		if (pawn == nullptr || pawn->IsPlayerControlled() || Cast<ACCharacter>(pawn))
		{
			continue;
		}

		UCHealthComponent* healthComp = Cast<UCHealthComponent>(pawn->GetComponentByClass(UCHealthComponent::StaticClass()));
		if (healthComp && healthComp->GetHealth() > 0.0f)
		{
			isAnyBotAlive = true;
			break;
		}
	}

	if (!isAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);

		PrepareForNextWave();
	}
}
