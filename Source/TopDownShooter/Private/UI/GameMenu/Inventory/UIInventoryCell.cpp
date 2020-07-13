// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Inventory/UIInventoryCell.h"
#include "UI/GameMenu/Inventory/UIInventoryDragNDropOperartion.h"
#include "UI/GameMenu/Inventory/UIInventoryContainer.h"
#include <Components/Image.h>
#include <Components/SizeBox.h>
#include "Inventory/InventoryContainer.h"
#include "UI/GameMenu/Inventory/UIInventoryFunctionLibrary.h"
#include <Engine/Engine.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetLayoutLibrary.h>

void UUIInventoryCell::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	UUIInventoryDragNDropOperartion* inventoryDragOperation = Cast<UUIInventoryDragNDropOperartion>(InOperation);
	if (inventoryDragOperation)
	{
		m_ownerContainer->OnCellDragEnter(inventoryDragOperation, m_cellPosition);
	}
}

void UUIInventoryCell::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	UUIInventoryDragNDropOperartion* inventoryDragOperation = Cast<UUIInventoryDragNDropOperartion>(InOperation);
	if (inventoryDragOperation)
	{
		m_ownerContainer->OnCellDragLeave(inventoryDragOperation, m_cellPosition);
	}
}

bool UUIInventoryCell::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UUIInventoryDragNDropOperartion* inventoryDragOperation = Cast<UUIInventoryDragNDropOperartion>(InOperation);
	if (inventoryDragOperation && inventoryDragOperation->m_fromItem)
	{
		UInventoryContainer* currentContainerModel = m_ownerContainer->GetModelContainer();
		FIntPoint correctedPlacePosition = UUIInventoryFunctionLibrary::GetCorrectedCellpositionForItem(m_cellPosition, inventoryDragOperation->m_fromItem->m_staticInfo->m_itemSize, currentContainerModel->GetSize());
		FInventoryDynamicItemInfo* movingItem = inventoryDragOperation->m_fromItem;
		return currentContainerModel->TryMoveItem(movingItem, correctedPlacePosition, inventoryDragOperation->GetIsRotated());
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UUIInventoryCell::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool UUIInventoryCell::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (GEngine)
	{
		FVector2D mousepos2 = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, *InGeometry.Position.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, *InGeometry.Size.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, *FString::SanitizeFloat(InGeometry.Scale));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, *InGeometry.GetAbsolutePosition().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, *mousepos2.ToString());
	}

	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

void UUIInventoryCell::SetParentContainer(UUIInventoryContainer* ownerContainer)
{
	ensure(ownerContainer && "cell should not exist withoput container!!!");
	m_ownerContainer = ownerContainer;
}

void UUIInventoryCell::SetCellPosition(FIntPoint& cellPosition)
{
	m_cellPosition = cellPosition;
}

void UUIInventoryCell::SetCellSize(FVector2D& cellSize)
{
	m_cellSizeBox->SetWidthOverride(cellSize.X);
	m_cellSizeBox->SetHeightOverride(cellSize.Y);
	m_cellFrameImage->SetBrushSize(cellSize);
	m_cellImage->SetBrushSize(cellSize);
}

void UUIInventoryCell::SetHighlight(const FLinearColor& cellColor)
{
	m_cellFrameImage->SetColorAndOpacity(cellColor);
	m_cellImage->SetColorAndOpacity(cellColor);
}
