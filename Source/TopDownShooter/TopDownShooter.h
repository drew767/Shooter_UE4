// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Kismet/GameplayStatics.h"

#define SURFACE_FLESHDEFAULT		SurfaceType1
#define SURFACE_FLESHVULNERABLE		SurfaceType2
#define SURFACE_FLESHLIMB     		SurfaceType3

#define COLLISION_WEAPON			ECC_GameTraceChannel1

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, text)
#define printFString1(fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, *fstring))
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))
	// Use the shortcut defined above
	//print("Hello Unreal");
	//printFString("My Variable Vector is: %s", *MyVector.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Y: %s %s %s"), *FString::SanitizeFloat(value), *FString::SanitizeFloat(m_rotationSensivity), *FString::SanitizeFloat(GetWorld()->GetDeltaSeconds())));
	//APawn::AddControllerPitchInput(value * m_rotationSensivity * GetWorld()->GetDeltaSeconds());
	//UE_LOG(LogTemp, Log, TEXT("Y: %s"), *FString::SanitizeFloat(value));


UENUM()
enum class EInventorySlot : uint8
{
	/* For currently equipped items/weapons */
	Hands,

	/* For primary weapons on spine bone */
	Primary,

	/* Storage for small items like flashlight on pelvis */
	Secondary,
};