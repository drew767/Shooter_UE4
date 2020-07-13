// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CustomCameraComponent.generated.h"

USTRUCT(BlueprintType)
struct FSCamerSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float m_interpolationSpeed = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_FOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FRotator m_rotation;

};

class ACustomizedCharacterInterface;
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UCustomCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UCustomCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay();

protected:
	UFUNCTION()
		void OnTargetingStateChangedEvent(bool isActive);

private:
	void InterpolateCameraTransform(ACustomizedCharacterInterface* pawn, float const DeltaTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interpolation")
		FSCamerSettings m_target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera State")
		FSCamerSettings m_normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera State")
		FSCamerSettings m_targeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpolation")
		bool m_lockFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpolation")
		bool m_lockPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpolation")
		bool m_lockRotation;
};
