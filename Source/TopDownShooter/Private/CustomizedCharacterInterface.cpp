// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizedCharacterInterface.h"
#include "TopDownShooter/TopDownShooter.h"
#include "CHealthComponent.h"
#include <GameFramework/Character.h>

// Sets default values
ACustomizedCharacterInterface::ACustomizedCharacterInterface()
{
	m_weaponHandSocketAttachPoint = "HandWeaponSocket";
	m_weaponPrimaryAttachPoint = "PrimaryWeaponSocket";
	m_weaponSecondaryAttachPoint = "SecondaryWeaponSocket";

	m_healthComponent = CreateDefaultSubobject<UCHealthComponent>(TEXT("HealthComponent"));
}

FName ACustomizedCharacterInterface::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	/* Return the socket name for the specified storage slot */
	switch (Slot)
	{
	case EInventorySlot::Hands:
		return m_weaponHandSocketAttachPoint;
	case EInventorySlot::Primary:
		return m_weaponPrimaryAttachPoint;
	case EInventorySlot::Secondary:
		return m_weaponSecondaryAttachPoint;
	default:
		// Not implemented.
		return "";
	}
}

// Called when the game starts or when spawned
void ACustomizedCharacterInterface::BeginPlay()
{
	Super::BeginPlay();

	m_healthComponent->m_onHealthChanged.AddDynamic(this, &ACustomizedCharacterInterface::OnHealthChanged);
}

void ACustomizedCharacterInterface::SetTargeting(bool isTargeting)
{
	m_isTargeting = isTargeting;
	m_OnTargetingStateChanged.Broadcast(isTargeting);
}

// Called every frame
void ACustomizedCharacterInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACustomizedCharacterInterface::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ACustomizedCharacterInterface::IsTargeting() const
{
	return m_isTargeting;
}

FRotator ACustomizedCharacterInterface::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

bool ACustomizedCharacterInterface::IsInitiatedJump() const
{
	return m_isJumping;
}

void ACustomizedCharacterInterface::SetIsJumping(bool isJumping)
{
	// Go to standing pose if trying to jump while crouched
	if (bIsCrouched && isJumping)
	{
		UnCrouch();
	}
	else if (isJumping != m_isJumping)
	{
		m_isJumping = isJumping;
		if (m_isJumping)
		{
			SetTargeting(false);
			ACharacter::Jump();
		}
	}
}

void ACustomizedCharacterInterface::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	/* Check if we are no longer falling/jumping */
	if (PrevMovementMode == EMovementMode::MOVE_Falling &&
		GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		SetIsJumping(false);
	}
}

void ACustomizedCharacterInterface::Jump()
{
	SetIsJumping(true);
}

void ACustomizedCharacterInterface::OnStartTargeting()
{
	//if (CarriedObjectComp->GetIsCarryingActor())
	//{
	//	CarriedObjectComp->Drop();
	//}
	SetTargeting(true);
}


void ACustomizedCharacterInterface::OnEndTargeting()
{
	SetTargeting(false);
}
