// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime/CoreUObject/Public/UObject/ObjectMacros.h>
#include "InventoryStaticItemInfo.h"
#include "GameFramework/Actor.h"
#include "InventoryDynamicItemInfo.generated.h"

class UInventoryContainer;

/**
 * 
 */
USTRUCT(BlueprintType)
struct TOPDOWNSHOOTER_API FInventoryDynamicItemInfo
{
    GENERATED_USTRUCT_BODY()

	uint64 m_uniqueItemId = 0;
	EInventoryItemId m_itemID = EInventoryItemId::None;
	uint16 m_count = 0;
	bool m_isRotated = false;
	FIntPoint m_position = FIntPoint::ZeroValue;

	AActor* m_owner = nullptr;
	UInventoryContainer* m_owningContainer = nullptr;
	FInventoryStaticItemInfo* m_staticInfo = nullptr;

	FORCEINLINE friend bool operator==(const FInventoryDynamicItemInfo& LHS, const FInventoryDynamicItemInfo& RHS)
	{
		bool idIsSame = LHS.m_uniqueItemId == RHS.m_uniqueItemId;
		bool dataIsSame = false;
		if (idIsSame)
		{
			dataIsSame = (LHS.m_itemID == RHS.m_itemID) && 
						 (LHS.m_count == RHS.m_count) &&
						 (LHS.m_isRotated == RHS.m_isRotated) &&
						 (LHS.m_position == RHS.m_position) &&
						 (LHS.m_owningContainer == RHS.m_owningContainer) &&
						 (LHS.m_owner == RHS.m_owner);

			ensure(dataIsSame);
		}
		return (idIsSame && dataIsSame);
	}
};