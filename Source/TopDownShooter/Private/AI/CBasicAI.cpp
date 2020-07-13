// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBasicAI.h"
#include <GameFramework/Character.h>
#include "TopDownShooter/TopDownShooter.h"
#include "CWeapon.h"

// Sets default values
ACBasicAI::ACBasicAI()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
}

// Called when the game starts or when spawned
void ACBasicAI::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_currentWeapon = GetWorld()->SpawnActor<ACWeapon>(m_starterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	if (m_currentWeapon)
	{
		m_currentWeapon->SetOwner(this);
		m_currentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_weaponAttachSocketName);
	}
}

// Called every frame
void ACBasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBasicAI::StartFire()
{
	if (m_currentWeapon)
	{
		m_currentWeapon->StartAIFire();
	}
}

void ACBasicAI::StopFire()
{
	if (m_currentWeapon)
	{
		m_currentWeapon->StopFire();
	}
}

void ACBasicAI::OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (health <= 0.0f)
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetLifeSpan(10.0f);
	}
}