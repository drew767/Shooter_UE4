// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIInventoryDragWidget.h"
#include "UIInventoryFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUIInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Swapping X and Y if needed"), Category = "Widget")
		static FIntPoint InvertPointIfNeeded(FIntPoint& vector, bool invert);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Swapping X and Y if needed"), Category = "Widget")
		static FVector2D InvertVectorIfNeeded(FVector2D& vector, bool invert);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Corrected position of left top corner of dragged item"), Category = "Widget")
		static FIntPoint GetCorrectedCellpositionForItem(const FIntPoint& triggeredCellPosition, const FIntPoint& itemSize, const FIntPoint& containerSize);
	 

	//UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Calculate Padding for UI Item according to it's position"), Category = "Widget")
	//static FMargin GetItemPaddingFromCellPosition(FIntPoint& cellPosition)


	//UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Create Inventory Drag Widget"), Category = "Widget")
	//	static UUIInventoryDragWidget* CreateDragWidget(UWidget* WorldContextObject);
};
