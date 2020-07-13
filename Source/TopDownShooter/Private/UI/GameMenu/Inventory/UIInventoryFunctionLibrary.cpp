// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Inventory/UIInventoryFunctionLibrary.h"


//FMargin UUIInventoryFunctionLibrary::GetItemPaddingFromCellPosition(FIntPoint& cellPosition) 
//{
//	float left = 0.0f;
//	float top = 0.0f;
//
//	left = m_cellsSize.X * cellPosition.X;
//	top = m_cellsSize.Y * cellPosition.Y;
//
//	FMargin result(left, top, 0.0f, 0.0f);
//	return result;
//}

/* */ // static UUIInventoryDragWidget* CreateDragWidget(UWidget* WorldContextObject)

FIntPoint UUIInventoryFunctionLibrary::InvertPointIfNeeded(FIntPoint& vector, bool invert) 
{
	if (invert)
	{
		return FIntPoint(vector.Y, vector.X);
	}
	else
	{
		return vector;
	}
}

FVector2D UUIInventoryFunctionLibrary::InvertVectorIfNeeded(FVector2D& vector, bool invert)
{
	if (invert)
	{
		return FIntPoint(vector.Y, vector.X);
	}
	else
	{
		return vector;
	}
}

FIntPoint UUIInventoryFunctionLibrary::GetCorrectedCellpositionForItem(const FIntPoint& triggeredCellPosition, const FIntPoint& itemSize, const FIntPoint& containerSize)
{
	FIntPoint itemOffset(itemSize.X / 2.0f, itemSize.Y / 2.0f);
	FIntPoint startingCell(triggeredCellPosition.X - itemOffset.X, triggeredCellPosition.Y - itemOffset.Y);

	if (startingCell.X + itemSize.X > containerSize.X)
	{
		startingCell.X = containerSize.X - itemSize.X;
	}

	if (startingCell.Y + itemSize.Y > containerSize.Y)
	{
		startingCell.Y = containerSize.Y - itemSize.Y;
	}

	if (startingCell.X < 0)
	{
		startingCell.X = 0;
	}

	if (startingCell.Y < 0)
	{
		startingCell.Y = 0;
	}

	return startingCell;
}
