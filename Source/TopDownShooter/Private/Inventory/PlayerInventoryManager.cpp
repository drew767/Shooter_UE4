// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PlayerInventoryManager.h"
#include "Inventory/InventoryContainer.h"
#include <UObject/ConstructorHelpers.h>

UPlayerInventoryManager::UPlayerInventoryManager()
{
}

FInventoryStaticItemInfo* UPlayerInventoryManager::GetStaticParamsForInventoryItem(EInventoryItemId id)
{
	if (m_mappedItemConfigs.Contains(id))
	{
		return &m_mappedItemConfigs[id];
	}
	ensure(false && "There is no such id in database!!!");
	return nullptr;
}

void UPlayerInventoryManager::PostInitProperties()
{
	Super::PostInitProperties();

	LoadInventoryItemsDbIntoSharedPointers();
	if (m_characterInventoryTemplate)
	{
		m_characterInventory = NewObject<UInventoryContainer>(this, m_characterInventoryTemplate);
	}
	if (m_onBaseInventoryTemplate)
	{
		m_onBaseInventory = NewObject<UInventoryContainer>(this, m_onBaseInventoryTemplate);
	}
}

void UPlayerInventoryManager::LoadInventoryItemsDbIntoSharedPointers()
{
	if (!m_inventoryItemConfigs)
	{
		return;
	}

	for (auto& it : m_inventoryItemConfigs->GetRowMap())
	{
		FString rowName = (it.Key).ToString();
		FInventoryStaticItemInfo* row = (FInventoryStaticItemInfo*)it.Value;
		m_mappedItemConfigs.Add(row->m_itemID, *row);
	}
}

bool UPlayerInventoryManager::AddItemToInventory(APickupItem* pickupItem)
{
	return m_characterInventory->AddItemToInventory(pickupItem);
}
