// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CustomSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UCustomSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
protected:
	/** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		uint32 bEnableCustomCameraLagSpeedForAxes : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta = (editcondition = "bEnableCustomCameraLagSpeedForAxes", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
		FVector CameraAxesLagSpeed;
};
