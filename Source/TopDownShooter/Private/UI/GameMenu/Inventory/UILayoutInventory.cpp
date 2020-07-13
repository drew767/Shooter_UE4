// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GameMenu/Inventory/UILayoutInventory.h"
#include <Components/VerticalBox.h>
#include "UI/GameMenu/Inventory/UIInventoryContainer.h"
#include "Services/CustomGameInstance.h"
#include "Inventory/InventoryContainer.h"
#include "Inventory/PlayerInventoryManager.h"
#include <Blueprint/WidgetBlueprintLibrary.h>

UUILayoutInventory::UUILayoutInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    UE_LOG(LogTemp, Log, TEXT("UUILayoutInventory::UUILayoutInventory()"));
}

void UUILayoutInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ensureMsgf(OnPlayerInventoryContainer, TEXT("UI visualization was not binded"));
	UPlayerInventoryManager* inventoryManager = Cast<UCustomGameInstance>(GetGameInstance())->GetInventorySystem();
	OnPlayerInventoryContainer->SetInventoryModel(inventoryManager->m_characterInventory);
}

void UUILayoutInventory::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;
	//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer());
	SetKeyboardFocus();
}

void  UUILayoutInventory::CreateWidgets()
{
	//if (m_playerTemplate)
	//{
	//	m_playerWidget = CreateWidget<UUIPlayerInventoryView>(GetWorld(), m_playerTemplate);
	//	UPanelSlot* playerWidgetSlot = PlayerContainer->AddChild(m_playerWidget);
	//}

	//if (m_containerTemplate)
	//{
	//	m_containerWidget = CreateWidget<UUIInventoryContainer>(GetWorld(), m_containerTemplate);
	//	UPanelSlot* inventoryWidgetSlot = InventoryContainer->AddChild(m_containerWidget);
	//}

	//if (m_lootingContainerTemplate)
	//{
	//	m_lootingContainerWidget = CreateWidget<UUIInventoryContainer>(GetWorld(), m_lootingContainerTemplate);
	//	UPanelSlot* lootWidgetSlot = LootedBoxContainer->AddChild(m_lootingContainerWidget);
	//}
}

void UUILayoutInventory::DestroyPlaceholders()
{
	//TArray<UWidget*> children = PlayerContainer->GetAllChildren();
	//children.Append(InventoryContainer->GetAllChildren());
	//children.Append(LootedBoxContainer->GetAllChildren());
	//for (auto* child : children)
	//{
	//	child->RemoveFromParent();
	//}

	//CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void UUILayoutInventory::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUILayoutInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//void UUILayoutInventory::SpawnCellsField()
//{
//    UUserWidget* new1Widget = CreateWidget(GetWorld(), m_cell);
//    new1Widget->SetPositionInViewport(FVector2D(-0.5f, -0.5f));
//    new1Widget->AddToViewport();
//  //  for (int i =0; i < 15; ++i)
//  //  {
//		//UUserWidget* newWidget = CreateWidget(GetWorld(), m_cell);
//		//newWidget->SetPositionInViewport(FVector2D(0 + (15.0f * i), 0 + (20.0f * i)));
//  //      newWidget->AddToViewport();
//  //  }
//}
