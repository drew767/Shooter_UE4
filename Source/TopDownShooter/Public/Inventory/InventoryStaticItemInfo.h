// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime/CoreUObject/Public/UObject/ObjectMacros.h>
#include <Engine/DataTable.h>
#include <Sound/SoundCue.h>
#include "InventoryEnums.h"
#include "InventoryStaticItemInfo.generated.h"

class UUIInventoryContainer;
/**
 * 
 */
USTRUCT(BlueprintType)
struct TOPDOWNSHOOTER_API FInventoryStaticItemInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
        EInventoryItemId m_itemID;
    UPROPERTY(EditDefaultsOnly)
        EInventoryItemType m_type;
    UPROPERTY(EditDefaultsOnly)
        bool m_canStack;
    UPROPERTY(EditDefaultsOnly)
        uint16 m_maxStack;
	UPROPERTY(EditDefaultsOnly)
        UTexture2D* m_image;
    UPROPERTY(EditDefaultsOnly)
        FIntPoint m_itemSize;
    UPROPERTY(EditDefaultsOnly)
        float m_weight; // kg per item
    UPROPERTY(EditDefaultsOnly)
        int64 m_price; // default price per item
    UPROPERTY(EditDefaultsOnly)
        FText m_name;
    UPROPERTY(EditDefaultsOnly)
        FText m_description;
    UPROPERTY(EditDefaultsOnly)
        USoundCue* m_dragSound;
    UPROPERTY(EditDefaultsOnly)
        USoundCue* m_dropSound;
    UPROPERTY(EditDefaultsOnly)
        bool m_isContainer;
    UPROPERTY(EditDefaultsOnly)
        FIntPoint m_containerSize;
    UPROPERTY(EditDefaultsOnly)
        TArray<EInventoryItemType> m_supportedItems;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> m_actorToDrop;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> m_actorToUse;
    UPROPERTY(EditDefaultsOnly)
        TArray<EInventoryItemId> m_supportedAttachmentIds;
    UPROPERTY(EditDefaultsOnly)
        TSubclassOf<UUIInventoryContainer> m_specialContainerWidgetTemplate;
};
