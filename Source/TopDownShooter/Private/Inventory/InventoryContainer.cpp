// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryContainer.h"
#include <Containers/BitArray.h>
#include <Engine/Engine.h>
#include "Services/CustomGameInstance.h"
#include "Inventory/PickupItem.h"
#include "Inventory/PlayerInventoryManager.h"
#include "UI/GameMenu/Inventory/UIInventoryFunctionLibrary.h"

void UInventoryContainer::Init()
{
	TArray<uint64> tmp;
	tmp.AddZeroed(m_size.X);
	for (uint16 i = 0; i < m_size.Y; ++i)
	{
		m_inventoryMap.Add(tmp);
	}

	PutAllItemsToPreviewMap();
}

void UInventoryContainer::AddItemToExactLocationInInventory(FInventoryDynamicItemInfo& item)
{
	for (auto& collectible : m_inventoryItemList)
	{
		if (collectible.m_uniqueItemId == item.m_uniqueItemId)
		{
			ensure(false && "Item with same id! Function was called for object wich is already in inventory or id generator was not used or generating broken ids");
		}
	}

	if (item.m_owner)
	{
		item.m_owner->Destroy();
		item.m_owner = nullptr;
	}
	FillInventoryMap(item, item.m_position);
	m_inventoryItemList.Add(item);
	item.m_owningContainer = this;

	m_onInventoryChanged.Broadcast();
}

void UInventoryContainer::PostInitProperties()
{
	Super::PostInitProperties();

	Init();
}

bool UInventoryContainer::AddItemToInventory(FInventoryDynamicItemInfo& item)
{
	for (auto& collectible : m_inventoryItemList)
	{
		if (collectible.m_uniqueItemId == item.m_uniqueItemId)
		{
			return false; 
		}
	}

	FIntPoint emptyPosition = FIntPoint::ZeroValue;
	bool needToRotate = false;
	bool emptyPlaceFound = FindEmptyPlace(item, emptyPosition, needToRotate);

	if (emptyPlaceFound)
	{
		if (item.m_owner)
		{		
			item.m_owner->Destroy();
			item.m_owner = nullptr;
		}
		item.m_isRotated = needToRotate;
		item.m_owningContainer = this;
		item.m_position  = emptyPosition;
		m_inventoryItemList.Add(item);
		FillInventoryMap(item, emptyPosition);

		m_onInventoryChanged.Broadcast();
		return true;
	}

	return false;
}

bool UInventoryContainer::AddItemToInventory(APickupItem* pickupItem)
{
	FInventoryDynamicItemInfo item;

	ensure(pickupItem);
	if (!pickupItem)
	{
		return false;
	}

	item.m_itemID = pickupItem->GetItemType();
	item.m_uniqueItemId = pickupItem->GetIdentifier();
	item.m_owner = pickupItem;
	item.m_staticInfo = UCustomGameInstance::GetInstance()->GetInventorySystem()->GetStaticParamsForInventoryItem(item.m_itemID);

	return AddItemToInventory(item);
}

void UInventoryContainer::RemoveFromInventory(FInventoryDynamicItemInfo& item)
{
	int32 index;
	if (m_inventoryItemList.Find(item, index))
	{
		item.m_isRotated = false;

		FillInventoryMap(item, item.m_position, true);
		m_inventoryItemList.Remove(item);
	}
}

void UInventoryContainer::RemoveFromInventoryById(uint64 id)
{
	int32 foundAtIndex = -1;
	for (int32 i = 0; i < m_inventoryItemList.Num(); ++i)
	{
		if (m_inventoryItemList[i].m_uniqueItemId == id)
		{
			foundAtIndex = i;
		}
	}

	ensure((foundAtIndex != -1) && "Failed to find item by index");

	if (foundAtIndex != -1)
	{
		FillInventoryMap(m_inventoryItemList[foundAtIndex], m_inventoryItemList[foundAtIndex].m_position, true);
		m_inventoryItemList.RemoveAt(foundAtIndex);
	}
}

bool UInventoryContainer::GetItem(int32 itemIndex, FInventoryDynamicItemInfo& outInfo)
{
	if (itemIndex >= 0 && m_inventoryItemList.Num() > itemIndex)
	{
		outInfo = m_inventoryItemList[itemIndex];
		return true;
	}
	return false;
}

void UInventoryContainer::PutAllItemsToPreviewMap()
{
	// clear map
	for (int i = 0; i < m_size.Y; ++i)
	{
		for (int j = 0; j < m_size.X; ++j)
		{
			m_inventoryMap[i][j] = 0;
		}
	}

	// put items on the map
	for (auto& collectible : m_inventoryItemList)
	{
		FillInventoryMap(collectible, collectible.m_position);
	}
}

void UInventoryContainer::FillInventoryMap(FInventoryDynamicItemInfo& item, FIntPoint const& position, bool remove)
{
	uint64 filler = remove ? 0 : item.m_uniqueItemId;

	FIntPoint fillingArea = item.m_staticInfo->m_itemSize;
	
	if (item.m_isRotated)
	{
		FIntPoint tmp = fillingArea;;
		fillingArea.X = tmp.Y;
		fillingArea.Y = tmp.X;
	}

	int finalY = position.Y + fillingArea.Y;
	int finalX = position.X + fillingArea.X;

	for (int32 row = position.Y; row < finalY; ++row)
	{
		for (int32 col = position.X; col < finalX; ++col)
		{
			m_inventoryMap[row][col] = filler;
		}
	}
}

bool UInventoryContainer::FindEmptyPlace(FInventoryDynamicItemInfo& item, FIntPoint& emptyPosition, bool& needToRotate)
{
	FIntPoint size = item.m_staticInfo->m_itemSize;
	FIntPoint position = FIntPoint::ZeroValue;
	for (int32 row = 0; row < m_inventoryMap.Num(); ++row)
	{
		position.Y = row;
		for (int32 col = 0; col < m_inventoryMap[row].Num(); ++col)
		{
			position.X = col;
			if (IsPositionValid(position, size))
			{
				emptyPosition = position;
				needToRotate = false;
				return true;
			}
		}
	}

	size = FIntPoint(size.Y, size.X);

	position = FIntPoint::ZeroValue;
	for (int32 row = 0; row < m_inventoryMap.Num(); ++row)
	{
		position.Y = row;
		for (int32 col = 0; col < m_inventoryMap[row].Num(); ++col)
		{
			position.X = col;
			if (IsPositionValid(position, size))
			{
				emptyPosition = position;
				needToRotate = true;
				return true;
			}
		}
	}

	return false;
}

bool UInventoryContainer::IsPositionValid(const FIntPoint& position, const FIntPoint& size, uint64 validFiller)
{
	if (position.Y >= m_inventoryMap.Num() ||
		position.X >= m_inventoryMap[position.Y].Num())
	{
		return false; // position is out of range
	}

	if (position.Y + size.Y > m_inventoryMap.Num() ||
		position.X + size.X > m_inventoryMap[position.X].Num())
	{
		return false; // object is to big for this position
	}

	int finalY = position.Y + size.Y;
	int finalX = position.X + size.X;

	for (int32 row = position.Y; row < finalY; ++row)
	{
		for (int32 col = position.X; col < finalX; ++col)
		{
			if (m_inventoryMap[row][col] != 0 && m_inventoryMap[row][col] != validFiller)
			{
				return false; // position colliding other objects
			}
		}
	}

	return true;
}

FIntPoint UInventoryContainer::GetSize()
{
	return m_size;
}

void UInventoryContainer::Print()
{
	if (GEngine)
	{
		for (int32 row = m_inventoryMap.Num() - 1; row >= 0; --row)
		{
			FString line = "";
			for (int32 col = 0; col < m_inventoryMap[row].Num(); ++col)
			{
				if (m_inventoryMap[row][col])
				{
					line += "#";
				}
				else
				{
					line += "_";
				}
			}
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, *line);
		}
	}
}

bool UInventoryContainer::TryMoveItem(FInventoryDynamicItemInfo* movingItem, FIntPoint& newPosition, bool overridenRotation)
{
	FIntPoint possessedSize = UUIInventoryFunctionLibrary::InvertPointIfNeeded(movingItem->m_staticInfo->m_itemSize, overridenRotation);
	if (IsPositionValid(newPosition, movingItem->m_staticInfo->m_itemSize, movingItem->m_uniqueItemId))
	{
		if (this != movingItem->m_owningContainer)
		{
			FInventoryDynamicItemInfo itemPossessedToNewContainer = *movingItem;
			movingItem->m_owningContainer->RemoveFromInventoryById(movingItem->m_uniqueItemId);

			itemPossessedToNewContainer.m_position = newPosition;
			itemPossessedToNewContainer.m_isRotated = overridenRotation;
			AddItemToExactLocationInInventory(itemPossessedToNewContainer);
		}
		else
		{
			movingItem->m_position = newPosition;
			movingItem->m_isRotated = overridenRotation;
			m_onInventoryChanged.Broadcast();
		}
		return true;
	}
	return false;
}

TArray<FInventoryDynamicItemInfo>& UInventoryContainer::GetItemsList()
{
	return m_inventoryItemList;
}