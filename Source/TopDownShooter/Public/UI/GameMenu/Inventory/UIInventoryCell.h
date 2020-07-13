// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInventoryCell.generated.h"

class UBorder;
class USizeBox;
class UOverlay;
class UImage;
class UTextBlock;
class UUIInventoryContainer;
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryCell : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	void SetParentContainer(UUIInventoryContainer* ownerContainer);
	void SetCellPosition(FIntPoint& cellPosition);
	void SetCellSize(FVector2D& cellSize);
	void SetHighlight(const FLinearColor& cellColor);

protected:
	UUIInventoryContainer* m_ownerContainer;
	FIntPoint m_cellPosition;

// BINDINGS
protected:
	UPROPERTY(meta = (BindWidgetOptional))
		UBorder* m_cellBorder;
	UPROPERTY(meta = (BindWidgetOptional))
		USizeBox* m_cellSizeBox;
	UPROPERTY(meta = (BindWidgetOptional))
		UOverlay* m_cellOverlay;
	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_cellFrameImage;
	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_cellImage;
};
