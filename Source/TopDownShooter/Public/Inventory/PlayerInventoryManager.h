// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryStaticItemInfo.h"
#include "PlayerInventoryManager.generated.h"

class UInventoryContainer;
class APickupItem;

UCLASS(BlueprintType, Blueprintable)
class TOPDOWNSHOOTER_API UPlayerInventoryManager : public UObject
{
	GENERATED_BODY()
public:
	UPlayerInventoryManager();
	FInventoryStaticItemInfo* GetStaticParamsForInventoryItem(EInventoryItemId id);
	virtual void PostInitProperties() override;
	bool AddItemToInventory(APickupItem* pickupItem);

protected:
	void LoadInventoryItemsDbIntoSharedPointers();

public:
	UPROPERTY()
		UInventoryContainer* m_characterInventory;
	UPROPERTY()
		UInventoryContainer* m_onBaseInventory;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory settings")
		TSubclassOf<UInventoryContainer> m_characterInventoryTemplate;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory settings")
		TSubclassOf<UInventoryContainer> m_onBaseInventoryTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configs")
		UDataTable* m_inventoryItemConfigs;

protected:
	UPROPERTY()
		TMap<EInventoryItemId, FInventoryStaticItemInfo> m_mappedItemConfigs;
};
