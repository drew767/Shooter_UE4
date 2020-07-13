// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Inventory/UIInventoryDragNDropOperartion.h"
#include "UI/GameMenu/Inventory/UIInventoryDragWidget.h"

bool UUIInventoryDragNDropOperartion::GetIsRotated()
{
	return Cast<UUIInventoryDragWidget>(DefaultDragVisual)->GetIsRotated();
}
