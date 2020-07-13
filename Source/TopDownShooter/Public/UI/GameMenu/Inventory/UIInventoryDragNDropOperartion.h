// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UIInventoryItem.h"
#include "UIInventoryDragNDropOperartion.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryDragNDropOperartion : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	FInventoryDynamicItemInfo* m_fromItem = nullptr;
	bool GetIsRotated();
};
