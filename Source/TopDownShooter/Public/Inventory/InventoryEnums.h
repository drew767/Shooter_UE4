// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime\CoreUObject\Public\UObject\ObjectMacros.h>

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	None = 0,
	Weapon,
	Clothes,
	Armor,
	Meds,
	Food,
	Drink,
	QuestItem,
	Ammo,
	Resource,
	Other,
};

UENUM(BlueprintType)
enum class EInventoryItemId : uint8
{
	None = 0,
	Rifle,
};

