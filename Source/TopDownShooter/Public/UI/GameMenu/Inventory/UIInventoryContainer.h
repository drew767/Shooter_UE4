// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInventoryCell.h"
#include "UIInventoryItem.h"
#include "UIInventoryContainer.generated.h"

class UOverlay;
class UUniformGridPanel;
class UBorder;
class UUIInventoryDragNDropOperartion;
class UInventoryContainer;

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	void OnCellDragEnter(UUIInventoryDragNDropOperartion* inventoryDragOperation, FIntPoint& cellPosition);
	void OnCellDragLeave(UUIInventoryDragNDropOperartion* inventoryDragOperation, FIntPoint& cellPosition);

	void SetInventoryModel(UInventoryContainer* inventory);
	UInventoryContainer* GetModelContainer() { return m_inventoryModel; }
	UUIInventoryItem* AddChildItemToContainer();
	FVector2D& GetCellSize();
	
protected:
	void SetHighlightToCells(FIntPoint& position, FIntPoint& size, bool highlight, bool isFree = false);
	void InitDefaultValues();
	void InitCellWidget();
	void ClearAllChildItems();
	UFUNCTION()
		void ReloadInventoryFromModel();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FVector2D m_cellsSize;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FLinearColor m_defaultCellColor;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FLinearColor m_availableHighlightCellColor;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FLinearColor m_occupiedHighlightCellColor;

private:
	TArray<UUIInventoryCell*> m_containerCells;
	TArray<UUIInventoryItem*> m_containerItems;
	UInventoryContainer* m_inventoryModel;

private:
// WIDGET TEMPLATES
	UPROPERTY(EditDefaultsOnly, Category = "Template")
		TSubclassOf<UUIInventoryCell> m_cellTemplate;

	UPROPERTY(EditDefaultsOnly, Category = "Template")
		TSubclassOf<UUIInventoryItem> m_itemTemplate;

// BINDINGS
	UPROPERTY(meta = (BindWidgetOptional))
		UUniformGridPanel* m_grid;

	UPROPERTY(meta = (BindWidgetOptional))
		UOverlay* m_overlay;

	UPROPERTY(meta = (BindWidgetOptional))
		UBorder* m_containerBorder;
};
