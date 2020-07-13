// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryDynamicItemInfo.h"
#include "UIInventoryItem.generated.h"

class UBorder;
class USizeBox;
class UOverlay;
class UImage;
class UTextBlock;
class UUIInventoryContainer;
class UOverlaySlot;

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void SynchronizeProperties() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void InitDefaultValues(FVector2D& cellSize); // NEED TO REFACTOR AND REMOVE THIS METHOD
	void CopyFromAnotherItem(UUIInventoryItem* fromItem, bool overridenRotation);
	void SetParentContainer(UUIInventoryContainer* ownerContainer);
	void SetModelObject(FInventoryDynamicItemInfo* item);
	FInventoryDynamicItemInfo* GetModelObject();
	void SetOverlaySlot(UOverlaySlot* slot) { m_overlayItemSlot = slot; }
	void FroceCustomPosition(FIntPoint& itemPosition);

protected:
	void SetPositionInternal(FIntPoint& itemPosition);
	void SetSizeInternal(FIntPoint& itemDimension, FVector2D& textureResolution);
	void SetSize(FVector2D& newSize);
	void EnableDragEffect();
	void DisableDragEffect();
	bool IsRotated();

protected:
	UUIInventoryContainer* m_ownerContainer;
	FInventoryDynamicItemInfo* m_modelObject;
	UOverlaySlot* m_overlayItemSlot;

	// BINDINGS
protected:
	UPROPERTY(meta = (BindWidgetOptional))
		UBorder* m_itemBorder;

	UPROPERTY(meta = (BindWidgetOptional))
		USizeBox* m_itemSizeBox;

	UPROPERTY(meta = (BindWidgetOptional))
		UOverlay* m_itemOverlay;

	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_itemFrameImage;

	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_itemImage;

	UPROPERTY(meta = (BindWidgetOptional))
		UTextBlock* m_textCount;

	UPROPERTY(meta = (BindWidgetOptional))
		UTextBlock* m_textItemName;	
};
