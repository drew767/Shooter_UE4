// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/UIService.h"
#include <Misc/AssertionMacros.h>
#include <Blueprint/UserWidget.h>
#include "CIngameCharacterPlayerController.h"
#include "UI/GameMenu/IMenuBaseLayout.h"

void AUIService::ShowLayout(EMenuPage const layoutID)
{
	verify(m_layouts.Contains(layoutID));

	if (!m_currentWidget)
	{
		m_currentWidget = CreateWidget<UIMenuBaseLayout>(GetWorld(), m_layouts[layoutID]);
		m_currentWidget->AddToViewport();

		ACIngameCharacterPlayerController* controller = Cast<ACIngameCharacterPlayerController>(GetGameInstance()->GetPrimaryPlayerController());
		if (controller)
		{
			controller->bShowMouseCursor = true;
			controller->bEnableClickEvents = true;
			controller->bEnableMouseOverEvents = true;
		}
	}
}

void AUIService::BeginPlay()
{
	Super::BeginPlay();

	//m_widgetLibrary = NewObject<UWidgetTemplatesLibrary>(this, m_widgetLibraryTemplate);

	//ShowLayout(EMenuPage::Inventory);
}
