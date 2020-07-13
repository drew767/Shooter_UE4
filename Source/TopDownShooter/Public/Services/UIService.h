// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetTemplatesLibrary.h"
#include "UIService.generated.h"

class UIMenuBaseLayout;

UENUM(BlueprintType, Blueprintable)
enum class EMenuPage : uint8
{
	None = 0,
	Inventory,
};

UCLASS()
class TOPDOWNSHOOTER_API AUIService : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
	void ShowLayout(EMenuPage const layoutID);

protected:
	UPROPERTY(EditAnywhere)
		UIMenuBaseLayout* m_currentWidget;

	UPROPERTY(EditAnywhere)
		TMap<EMenuPage, TSubclassOf<UIMenuBaseLayout>> m_layouts;

public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Library")
		TSubclassOf<UWidgetTemplatesLibrary> m_widgetLibraryTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Library")
		UWidgetTemplatesLibrary* m_widgetLibrary;
};


//// TEMPLATES OF WIDGETS TO CREATE
//protected:
//	UPROPERTY(EditAnywhere, Category = "Template")
//		TSubclassOf<UUIInventoryContainer> m_containerTemplate;
//
//	UPROPERTY(EditAnywhere, Category = "Template")
//		TSubclassOf<UUIPlayerInventoryView> m_playerTemplate;
//
//	UPROPERTY(EditAnywhere, Category = "Template")
//		TSubclassOf<UUIInventoryContainer> m_lootingContainerTemplate;
//
//	// WIDGETS WHICH WILL BE DYNAMICALY CREATEAD START HERE
//private:
//	UPROPERTY(EditAnywhere, Category = "Instances")
//		UUIInventoryContainer* m_containerWidget;
//
//	UPROPERTY(EditAnywhere, Category = "Instances")
//		UUIPlayerInventoryView* m_playerWidget;
//
//	UPROPERTY(EditAnywhere, Category = "Instances")
//		UUIInventoryContainer* m_lootingContainerWidget;
//
//	// BINDED CONTAINERS FOR WIDGETS STARTS HERE
//private:
//	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
//		UUIInventoryContainer* InventoryContainer;
//
//	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
//		UUIInventoryContainer* LootedBoxContainer;