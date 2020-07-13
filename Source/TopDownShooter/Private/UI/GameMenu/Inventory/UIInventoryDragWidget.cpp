// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Inventory/UIInventoryDragWidget.h"
#include <Components/SizeBox.h>
#include <Components/Image.h>
#include <Materials/MaterialInstanceDynamic.h>

void UUIInventoryDragWidget::InitDragWidget(UTexture* itemTexture, UTexture* frameTexture, FVector2D& size, bool isRotated)
{
	ensure(itemTexture);
	
	m_dragImage->SetBrushSize(size);
	m_frameImage->SetBrushSize(size);

	FName textureParamName = "Image";
	FName rotationParamName = "Rotation";

	UMaterialInstanceDynamic* itemMaterial = m_dragImage->GetDynamicMaterial();
	m_isRotated = isRotated;
	float rotation = m_isRotated == 0 ? 0.0f : 0.25f;

	itemMaterial->SetScalarParameterValue(rotationParamName, rotation);
	itemMaterial->SetTextureParameterValue(textureParamName, itemTexture);
	
	if (frameTexture)
	{
		UMaterialInstanceDynamic* frameMaterial = m_frameImage->GetDynamicMaterial();
		frameMaterial->SetTextureParameterValue(textureParamName, frameTexture);
	}
}

bool UUIInventoryDragWidget::GetIsRotated()
{
	return m_isRotated;
}
