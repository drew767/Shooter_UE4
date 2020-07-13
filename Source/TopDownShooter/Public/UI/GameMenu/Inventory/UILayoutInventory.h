// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GameMenu/IMenuBaseLayout.h"
#include "UILayoutInventory.generated.h"

class UVerticalBox;
class UUIInventoryContainer;
class UUIPlayerInventoryView;
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UUILayoutInventory : public UIMenuBaseLayout
{
	GENERATED_BODY()
	
public:
    UUILayoutInventory(const FObjectInitializer& ObjectInitializer);
    virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CreateWidgets();
	void DestroyPlaceholders();

// BINDED CONTAINERS FOR WIDGETS STARTS HERE
private: 
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
		UUIInventoryContainer* OnPlayerInventoryContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
		UUIInventoryContainer* LootedInventoryContainer;
};
