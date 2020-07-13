// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Inventory/UIInventoryContainer.h"

#include <Components/UniformGridPanel.h>
#include <Components/Overlay.h>
#include <Components/OverlaySlot.h>
#include "UI/GameMenu/Inventory/UIInventoryDragNDropOperartion.h"
#include "Inventory/InventoryContainer.h"
#include "UI/GameMenu/Inventory/UIInventoryFunctionLibrary.h"

void UUIInventoryContainer::NativeConstruct()
{
	Super::NativeConstruct();
}

UUIInventoryItem* UUIInventoryContainer::AddChildItemToContainer()
{
	UUIInventoryItem* item = CreateWidget<UUIInventoryItem>(this, m_itemTemplate);
	m_containerItems.Push(item);
	item->SetOverlaySlot(m_overlay->AddChildToOverlay(item));
	item->SetParentContainer(this);
	return item;
}

bool UUIInventoryContainer::Initialize()
{
	bool result = Super::Initialize();
	
	InitDefaultValues();
	return result;
}

void UUIInventoryContainer::OnCellDragEnter(UUIInventoryDragNDropOperartion* inventoryDragOperation, FIntPoint& cellPosition)
{
	FInventoryDynamicItemInfo* modelObject = inventoryDragOperation->m_fromItem;
	FIntPoint realPos = UUIInventoryFunctionLibrary::GetCorrectedCellpositionForItem(cellPosition, modelObject->m_staticInfo->m_itemSize, m_inventoryModel->GetSize());
	bool isFree = m_inventoryModel->IsPositionValid(realPos, modelObject->m_staticInfo->m_itemSize, modelObject->m_uniqueItemId);
	SetHighlightToCells(realPos, modelObject->m_staticInfo->m_itemSize, true, isFree);
}

void UUIInventoryContainer::OnCellDragLeave(UUIInventoryDragNDropOperartion* inventoryDragOperation, FIntPoint& cellPosition)
{
	FInventoryDynamicItemInfo* modelObject = inventoryDragOperation->m_fromItem;
	FIntPoint realPos = UUIInventoryFunctionLibrary::GetCorrectedCellpositionForItem(cellPosition, modelObject->m_staticInfo->m_itemSize, m_inventoryModel->GetSize());
	SetHighlightToCells(realPos, modelObject->m_staticInfo->m_itemSize, false);
}

void UUIInventoryContainer::SetHighlightToCells(FIntPoint& position, FIntPoint& size, bool highlight, bool isFree)
{
	int16 lastRow = position.Y + size.Y;
	lastRow = lastRow < m_inventoryModel->GetSize().Y ? lastRow : m_inventoryModel->GetSize().Y;

	int16 lastCol = position.X + size.X;
	lastCol = lastCol < m_inventoryModel->GetSize().X ? lastCol : m_inventoryModel->GetSize().X;

	for (int16 row = position.Y; row < lastRow; ++row)
	{
		for (int16 col = position.X; col < lastCol; ++col)
		{
			int16 index = row * m_inventoryModel->GetSize().X + col;
			if (highlight)
			{
				if (isFree)
				{
					m_containerCells[index]->SetHighlight(m_availableHighlightCellColor);
				}
				else
				{
					m_containerCells[index]->SetHighlight(m_occupiedHighlightCellColor);
				}
			}
			else
			{
				m_containerCells[index]->SetHighlight(m_defaultCellColor);
			}
		}
	}	
}

void UUIInventoryContainer::SetInventoryModel(UInventoryContainer* inventory)
{
	m_inventoryModel = inventory;
	m_inventoryModel->m_onInventoryChanged.AddDynamic(this, &UUIInventoryContainer::ReloadInventoryFromModel);
	ReloadInventoryFromModel();
}

void UUIInventoryContainer::ReloadInventoryFromModel()
{
	TArray<FInventoryDynamicItemInfo>& items = m_inventoryModel->GetItemsList();
	ClearAllChildItems();
	InitCellWidget();

	for (auto& item : items)
	{
		UUIInventoryItem* uiItem = AddChildItemToContainer();
		uiItem->SetModelObject(&item);
	}
}

void UUIInventoryContainer::ClearAllChildItems()
{
	for (auto& item : m_containerItems)
	{
		item->RemoveFromParent();
	}
	for (auto& cell : m_containerCells)
	{
		cell->RemoveFromParent();
	}
	m_containerCells.Empty();
	m_containerItems.Empty();
}

FVector2D& UUIInventoryContainer::GetCellSize()
{
	return m_cellsSize;
}

void UUIInventoryContainer::InitDefaultValues()
{
	if (IsDesignTime())
	{

		if (!m_grid || 
			!m_overlay || 
			!m_containerBorder ||
			!m_cellTemplate || 
			!m_itemTemplate)
		{
			ensure(false && "Bindings and/or templates are invalid");
			return;
		}

		FIntPoint tmpSize(5, 5);
		for (int i = 0; i < tmpSize.Y; ++i)
		{
			for (int j = 0; j < tmpSize.X; ++j)
			{
				UUIInventoryCell* newWidget = CreateWidget<UUIInventoryCell>(this, m_cellTemplate);
				newWidget->SetParentContainer(this);
				newWidget->SetCellSize(m_cellsSize);
				FIntPoint cellPosition(j, i);
				newWidget->SetCellPosition(cellPosition);
				m_grid->AddChildToUniformGrid(newWidget, i, j);
				m_containerCells.Push(newWidget);
			}
		}
		FIntPoint itemPosition1(0, 0);
		FIntPoint itemPosition2(2, 3);
		FIntPoint itemPosition3(4, 3);
		AddChildItemToContainer();
		AddChildItemToContainer();
		AddChildItemToContainer();
		m_containerItems[0]->FroceCustomPosition(itemPosition1);
		m_containerItems[1]->FroceCustomPosition(itemPosition2);
		m_containerItems[2]->FroceCustomPosition(itemPosition3);
	}
}

void UUIInventoryContainer::InitCellWidget()
{
	FIntPoint size = m_inventoryModel->GetSize();
	ensure(m_containerCells.Num() == 0 && "All cells should be cleared before calling this function");
	m_containerCells.SetNum(size.X * size.Y);

	int count = 0;
	for (int i = 0; i < size.Y; ++i)
	{
		for (int j = 0; j < size.X; ++j)
		{
			UUIInventoryCell* newWidget = CreateWidget<UUIInventoryCell>(this, m_cellTemplate);
			newWidget->SetParentContainer(this);
			newWidget->SetCellSize(m_cellsSize);
			FIntPoint cellPosition(j, i);
			newWidget->SetCellPosition(cellPosition);
			m_grid->AddChildToUniformGrid(newWidget, i, j);
			m_containerCells[count] = newWidget;
			++count;
		}
	}
}
