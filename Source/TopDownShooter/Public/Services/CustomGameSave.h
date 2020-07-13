// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomGameSave.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UCustomGameSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		int m_gameId;

private:
	UPROPERTY()
		int32 m_savedDataVersion;
};
