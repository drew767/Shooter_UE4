// Fill out your copyright notice in the Description page of Project Settings.

#include "CRaidGameState.h"
#include <Net/UnrealNetwork.h>
#include <UObject/CoreNet.h>

void ACRaidGameState::OnRep_WaveState(EWaveState oldState)
{
	WaveStateChanged(m_waveState, oldState);
}

void ACRaidGameState::SetWaveState(EWaveState newState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		EWaveState oldState = m_waveState;

		m_waveState = newState;
		// Call on server
		OnRep_WaveState(oldState);
	}
}

void ACRaidGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRaidGameState, m_waveState);
}
