// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInputHandler.h"
#include "IMenuBaseLayout.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UIMenuBaseLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	TSharedPtr<UIInputHandler> m_inputHandler;
};
