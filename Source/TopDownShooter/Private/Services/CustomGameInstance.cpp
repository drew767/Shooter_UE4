// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/CustomGameInstance.h"
#include "Inventory/PlayerInventoryManager.h"

UCustomGameInstance* UCustomGameInstance::m_instance = nullptr;

UCustomGameInstance::UCustomGameInstance()
{
	m_idGenerator = 1;
	m_instance = this;
}

void UCustomGameInstance::Init()
{
	Super::Init();
	m_inventorySys = NewObject<UPlayerInventoryManager>(this, m_inventorySysTemplate);
}

uint64 UCustomGameInstance::GetNewObjectID()
{
	return m_idGenerator++;
}

UPlayerInventoryManager* UCustomGameInstance::GetInventorySystem()
{
	return m_inventorySys;
}

UWorld* UCustomGameInstance::GetWorldPointer()
{
	if (m_instance)
	{
		return GEngine->GetWorldFromContextObjectChecked(m_instance);
	}
	else
	{
		return nullptr;
	}
}

UCustomGameInstance* UCustomGameInstance::GetInstance()
{
	return m_instance;
}

