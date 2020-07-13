// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInventoryDragWidget.generated.h"

class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryDragWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitDragWidget(UTexture* itemTexture, UTexture* frameTexture, FVector2D& size, bool isRotated);
	bool GetIsRotated();

protected:
	bool m_isRotated;

	// BINDINGS	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
		UBorder* m_dragBorder;
	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_dragImage;
	UPROPERTY(meta = (BindWidgetOptional))
		UImage* m_frameImage;
};
