// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Delegates/DelegateCombinations.h>
#include "InventoryEnums.h"
#include "InventoryDynamicItemInfo.h"
#include "InventoryContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryContentChanged);

class UConstCollectibleConfig;
class ACollectiblePhysicWorldItem;
class UInventoryConfig;
class APickupItem;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TOPDOWNSHOOTER_API UInventoryContainer : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void PostInitProperties() override;

	bool AddItemToInventory(APickupItem* pickupItem);
	bool AddItemToInventory(FInventoryDynamicItemInfo& item);
	void RemoveFromInventory(FInventoryDynamicItemInfo& item);
	void RemoveFromInventoryById(uint64 id);
	TArray<FInventoryDynamicItemInfo>& GetItemsList();
	bool TryMoveItem(FInventoryDynamicItemInfo* movingItem, FIntPoint& newPosition, bool overridenRotation);
	bool GetItem(int32 itemIndex, FInventoryDynamicItemInfo& outInfo);
	bool IsPositionValid(const FIntPoint& position, const FIntPoint& size, uint64 validFiller = 0);

	FIntPoint GetSize();
	void Print();

protected:
	bool FindEmptyPlace(FInventoryDynamicItemInfo& item, FIntPoint& emptyPosition, bool& needToRotate);
	void AddItemToExactLocationInInventory(FInventoryDynamicItemInfo& item);
	void PutAllItemsToPreviewMap();
	void FillInventoryMap(FInventoryDynamicItemInfo& item, FIntPoint const& position, bool remove = false);
	void Init();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<FInventoryDynamicItemInfo> m_inventoryItemList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
		FIntPoint m_size = FIntPoint::ZeroValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
		TArray<EInventoryItemType> m_supportedItemTypes;
private:
	TArray<TArray<uint64>> m_inventoryMap;
	uint64 m_uniqueId;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FInventoryContentChanged m_onInventoryChanged;
};
