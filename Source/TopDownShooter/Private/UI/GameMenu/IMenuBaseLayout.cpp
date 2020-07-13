// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/IMenuBaseLayout.h"
#include "UI/UIInputHandler.h"
#include <Framework/Application/SlateApplication.h>

void UIMenuBaseLayout::NativeConstruct()
{
	Super::NativeConstruct();

	m_inputHandler = MakeShareable(new UIInputHandler());
	FSlateApplication::Get().RegisterInputPreProcessor(m_inputHandler);
}

void UIMenuBaseLayout::NativeDestruct()
{
	Super::NativeDestruct();
	FSlateApplication::Get().UnregisterInputPreProcessor(m_inputHandler);
}

