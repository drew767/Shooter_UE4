// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
		void AddScore(float scoreDelta);
};
