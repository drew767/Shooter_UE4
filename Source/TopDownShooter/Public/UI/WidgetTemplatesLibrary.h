// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameMenu/Inventory/UIInventoryCell.h"
#include "GameMenu/Inventory/UIInventoryItem.h"
#include "GameMenu/Inventory/UIInventoryDragWidget.h"
#include "WidgetTemplatesLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TOPDOWNSHOOTER_API UWidgetTemplatesLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Template")
		TSubclassOf<UUIInventoryDragWidget> m_inventoryDragWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Template")
		TSubclassOf<UUIInventoryItem> m_inventoryItemWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Template")
		TSubclassOf<UUIInventoryCell> m_inventoryCellWidget;
};
