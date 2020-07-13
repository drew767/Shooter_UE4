// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInputHandler.h"
#include "UI/GameMenu/IMenuBaseLayout.h"
#include <Engine/Engine.h>

UIInputHandler::UIInputHandler()
{
}

UIInputHandler::~UIInputHandler()
{
}

void UIInputHandler::SetFocusedLayout(UIMenuBaseLayout* focusedLayout)
{
	//if (m_focusedLayout)
	//{
	//	m_focusedLayout->UnsubscribeInput(this);
	//}

	//m_focusedLayout = focusedLayout;
	//m_focusedLayout->SubscribeInput(this);
}

void UIInputHandler::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, "THAT IS MOUSE LOCATION VALUE I NEEDED");
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, *m_mousePos.ToString());
	}
}

bool UIInputHandler::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return IInputProcessor::HandleKeyUpEvent(SlateApp, InKeyEvent);
}

bool UIInputHandler::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return IInputProcessor::HandleKeyUpEvent(SlateApp, InKeyEvent);
}

bool UIInputHandler::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
	return IInputProcessor::HandleAnalogInputEvent(SlateApp, InAnalogInputEvent);
}

bool UIInputHandler::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	m_mousePos = MouseEvent.GetScreenSpacePosition();
	
	return IInputProcessor::HandleMouseMoveEvent(SlateApp, MouseEvent);
}

bool UIInputHandler::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return IInputProcessor::HandleMouseButtonDownEvent(SlateApp, MouseEvent);
}

bool UIInputHandler::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return IInputProcessor::HandleMouseButtonUpEvent(SlateApp, MouseEvent);
}

bool UIInputHandler::HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return IInputProcessor::HandleMouseButtonDoubleClickEvent(SlateApp, MouseEvent);
}

bool UIInputHandler::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent)
{
	return IInputProcessor::HandleMouseWheelOrGestureEvent(SlateApp, InWheelEvent, InGestureEvent);
}

bool UIInputHandler::HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent)
{
	return IInputProcessor::HandleMotionDetectedEvent(SlateApp, MotionEvent);
}
