// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CustomCameraComponent.h"
#include "CustomizedCharacterInterface.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_normal.m_interpolationSpeed = 20.0f;
	m_normal.m_FOV = 90.0f;
	m_normal.m_position = FVector(-70.0f, 20.0f, 5.0f);
	m_normal.m_rotation = FRotator(0.0f, 0.0f, 0.0f);

	m_targeting.m_interpolationSpeed = 20.0f;
	m_targeting.m_FOV = 40.0f;
	m_targeting.m_position = FVector(0.0f, 20.0f, 20.0f);
	m_targeting.m_rotation = FRotator(0.0f, 0.0f, 0.0f);

	m_target = m_normal;
}


void UCustomCameraComponent::OnTargetingStateChangedEvent(bool isActive)
{
	if (isActive)
	{
		m_target = m_targeting;
	}
	else
	{
		m_target = m_normal;
	}
}

// Called every frame
void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACustomizedCharacterInterface* pawn = Cast<ACustomizedCharacterInterface>(GetOwner());

	InterpolateCameraTransform(pawn, DeltaTime);
}

void UCustomCameraComponent::InterpolateCameraTransform(ACustomizedCharacterInterface* pawn, float const DeltaTime)
{
	if (pawn)
	{
		if (!m_lockFOV)
		{
			SetFieldOfView(FMath::FInterpTo(FieldOfView, m_target.m_FOV, DeltaTime, m_target.m_interpolationSpeed));
		}
		if (!m_lockRotation)
		{
			SetRelativeRotation(FMath::RInterpTo(GetRelativeRotation(), m_target.m_rotation, DeltaTime, m_target.m_interpolationSpeed));
		}
		if (!m_lockPosition)
		{
			SetRelativeLocation(FMath::VInterpTo(GetRelativeLocation(), m_target.m_position, DeltaTime, m_target.m_interpolationSpeed));
		}
	}
}

void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	SetFieldOfView(m_normal.m_FOV);
	SetRelativeRotation(m_normal.m_rotation);
	SetRelativeLocation(m_normal.m_position);

	Cast<ACustomizedCharacterInterface>(GetOwner())->m_OnTargetingStateChanged.AddDynamic(this, &UCustomCameraComponent::OnTargetingStateChangedEvent);
}
