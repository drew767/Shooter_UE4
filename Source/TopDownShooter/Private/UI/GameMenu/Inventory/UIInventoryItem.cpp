// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GameMenu/Inventory/UIInventoryItem.h"

#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Components/Image.h>
#include <Components/SizeBox.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "UI/GameMenu/Inventory/UIInventoryDragNDropOperartion.h"
#include "UI/GameMenu/Inventory/UIInventoryDragWidget.h"
#include "UI/GameMenu/Inventory/UIInventoryFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Services/UIService.h"
#include <InputCoreTypes.h>
#include <Framework/Application/SlateApplication.h>
#include <Components/Overlay.h>
#include "Inventory/InventoryContainer.h"
#include <Components/OverlaySlot.h>
#include "UI/GameMenu/Inventory/UIInventoryContainer.h"

bool UUIInventoryItem::Initialize()
{
	bool result = Super::Initialize();
	return result;
}

FReply UUIInventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) 
	{
		if (!m_modelObject)
		{
			ensure(false && "Item widget has no model!");
			UE_LOG(LogTemp, Warning, TEXT("Item widget has no model! %s"), *GetName());
			return FReply::Unhandled();
		}
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Unhandled();
}

void UUIInventoryItem::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

//FReply UUIInventoryItem::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
//{
//	if (m_isDragged)
//	{
//		FKey cancelKey = EKeys::Escape;
//
//#if WITH_EDITOR
//		cancelKey = EKeys::CapsLock;
//#endif
//
//		if (InKeyEvent.GetKey() == cancelKey)
//		{
//			UWidgetBlueprintLibrary::CancelDragDrop();
//			return FReply::Handled();
//		}
//	}
//
//	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
//}

void UUIInventoryItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UUIInventoryDragNDropOperartion::StaticClass());
	if (OutOperation && m_modelObject)
	{
		UUIInventoryDragNDropOperartion* operation = Cast<UUIInventoryDragNDropOperartion>(OutOperation);
		operation->m_fromItem = m_modelObject;
		EnableDragEffect();

		AUIService* hud = Cast<AUIService>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		UUIInventoryDragWidget* dragWidget = CreateWidget<UUIInventoryDragWidget>(this, hud->m_widgetLibrary->m_inventoryDragWidget);

		FName textureParamName = "Image";
		UMaterialInstanceDynamic* itemMaterial = m_itemImage->GetDynamicMaterial();
		UTexture* itemTexture;
		itemMaterial->GetTextureParameterValue(textureParamName, itemTexture);
		
		FVector2D size(m_itemSizeBox->WidthOverride, m_itemSizeBox->HeightOverride);
		dragWidget->InitDragWidget(itemTexture, nullptr, size, m_modelObject->m_isRotated);
		operation->DefaultDragVisual = dragWidget;
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UUIInventoryItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UUIInventoryDragNDropOperartion* inventoryDragOperation = Cast<UUIInventoryDragNDropOperartion>(InOperation);
	if (inventoryDragOperation)
	{
		SynchronizeProperties();
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UUIInventoryItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	DisableDragEffect();
}

void UUIInventoryItem::CopyFromAnotherItem(UUIInventoryItem* fromItem, bool overridenRotation)
{
	FVector2D size(fromItem->m_itemSizeBox->WidthOverride, fromItem->m_itemSizeBox->HeightOverride);
	SetSize(size);

	m_modelObject = fromItem->m_modelObject;
	m_modelObject->m_isRotated = overridenRotation;
	FName rotationParamName = "Rotation";
	FName textureParamName = "Image";

	UMaterialInstanceDynamic* materialFrom = fromItem->m_itemImage->GetDynamicMaterial();
	UMaterialInstanceDynamic* materialTo = m_itemImage->GetDynamicMaterial();

	UTexture* itemTexture;
	materialFrom->GetTextureParameterValue(textureParamName, itemTexture);
	materialTo->SetTextureParameterValue(textureParamName, itemTexture);

	float rotation = m_modelObject->m_isRotated == 0 ? 0.0f : 0.25f;
	materialTo->SetScalarParameterValue(rotationParamName, rotation);
}

void UUIInventoryItem::SetParentContainer(UUIInventoryContainer* ownerContainer)
{
	ensure(ownerContainer && "item should not exist withoput container!!!");
	m_ownerContainer = ownerContainer;
}

void UUIInventoryItem::SetSize(FVector2D& newSize)
{
	m_itemSizeBox->SetWidthOverride(newSize.X);
	m_itemSizeBox->SetHeightOverride(newSize.Y);
	m_itemImage->SetBrushSize(newSize);
	m_itemFrameImage->SetBrushSize(newSize);
}

void UUIInventoryItem::InitDefaultValues(FVector2D& cellSize)
{
	FVector2D vec = FVector2D(0.0f, 0.0f);
	if (IsDesignTime())
	{
		switch (FMath::RandRange(0, 5))
		{
		case 0:
		{
			vec.X = cellSize.X * 1;
			vec.Y = cellSize.Y * 1;
			break;
		}
		case 1:
		{
			vec.X = cellSize.X * 2;
			vec.Y = cellSize.Y * 1;
			break;
		}
		case 2:
		{
			vec.X = cellSize.X * 1;
			vec.Y = cellSize.Y * 2;
			break;
		}
		case 3:
		{
			vec.X = cellSize.X * 3;
			vec.Y = cellSize.Y * 1;
			break;
		}
		case 4:
		{
			vec.X = cellSize.X * 3;
			vec.Y = cellSize.Y * 2;
			break;
		}
		case 5:
		{
			vec.X = cellSize.X * 2;
			vec.Y = cellSize.Y * 3;
			break;
		}
		default:
			break;
		}


		m_itemSizeBox->SetWidthOverride(vec.X);
		m_itemSizeBox->SetHeightOverride(vec.Y);
		m_itemImage->SetBrushSize(vec);

		UMaterialInstanceDynamic* material = m_itemImage->GetDynamicMaterial();
		FName rotationParamName = "Rotation";
		bool isRotated = FMath::RandRange(0, 1) == 0;
		float rotation = isRotated == 0 ? 0.0f : 0.25f;
		material->SetScalarParameterValue(rotationParamName, rotation);
	}
}

void UUIInventoryItem::SetModelObject(FInventoryDynamicItemInfo* item)
{
	m_modelObject = item;

	SetPositionInternal(m_modelObject->m_position);

	UTexture2D* texture = m_modelObject->m_staticInfo->m_image;
	FVector2D textureSize(texture->GetSizeX(), texture->GetSizeY());
	textureSize = UUIInventoryFunctionLibrary::InvertVectorIfNeeded(textureSize, m_modelObject->m_isRotated);
	SetSizeInternal(m_modelObject->m_staticInfo->m_itemSize, textureSize);

	UMaterialInstanceDynamic* material = m_itemImage->GetDynamicMaterial();
	FName rotationParamName = "Rotation";

	float rotation = m_modelObject->m_isRotated == 0 ? 0.0f : 0.25f;
	material->SetScalarParameterValue(rotationParamName, rotation);

	FName textureParamName = "Image";
	material->SetTextureParameterValue(textureParamName, m_modelObject->m_staticInfo->m_image);
}

FInventoryDynamicItemInfo* UUIInventoryItem::GetModelObject()
{
	return m_modelObject;
}

void UUIInventoryItem::EnableDragEffect()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	float alpha = 0.5f;
	m_itemImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alpha));
	m_itemFrameImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alpha));
}

void UUIInventoryItem::DisableDragEffect()
{
	SetVisibility(ESlateVisibility::Visible);
	m_itemImage->SetColorAndOpacity(FLinearColor::White);
	m_itemFrameImage->SetColorAndOpacity(FLinearColor::White);
}

bool UUIInventoryItem::IsRotated()
{
	if (m_modelObject)
	{
		return m_modelObject->m_isRotated;
	}
	return false;
}

void UUIInventoryItem::FroceCustomPosition(FIntPoint& itemPosition)
{
	SetPositionInternal(itemPosition);
}

void UUIInventoryItem::SetPositionInternal(FIntPoint& itemPosition)
{
	float left = 0.0f;
	float top = 0.0f;

	FVector2D& cellSize = m_ownerContainer->GetCellSize();

	left = cellSize.X * itemPosition.X;
	top = cellSize.Y * itemPosition.Y;

	m_overlayItemSlot->SetPadding(FMargin(left, top, 0.0f, 0.0f));
}

void UUIInventoryItem::SetSizeInternal(FIntPoint& itemDimension, FVector2D& textureResolution)
{
	FVector2D& cellSize = m_ownerContainer->GetCellSize();

	FVector2D uiItemSize;
	if (!m_modelObject->m_isRotated)
	{
		uiItemSize = cellSize * itemDimension;
	}
	else
	{
		uiItemSize.X = cellSize.X * itemDimension.Y;
		uiItemSize.Y = cellSize.Y * itemDimension.X;
	}
	m_itemSizeBox->SetWidthOverride(uiItemSize.X);
	m_itemSizeBox->SetHeightOverride(uiItemSize.Y);

	m_itemImage->SetBrushSize(textureResolution);
}