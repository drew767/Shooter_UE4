// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Engine/Engine.h>
#include "CustomGameInstance.generated.h"

class UPlayerInventoryManager;
class UDataTable;
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCustomGameInstance();
	virtual void Init() override;
	uint64 GetNewObjectID();
	UPlayerInventoryManager* GetInventorySystem();

	static UWorld* GetWorldPointer();
	static UCustomGameInstance* GetInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInfo")
		int m_gameId;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryManager")
		TSubclassOf<UPlayerInventoryManager> m_inventorySysTemplate;

	UPROPERTY()
		UPlayerInventoryManager* m_inventorySys;
private:
	static UCustomGameInstance* m_instance;
	uint64 m_idGenerator;
};
