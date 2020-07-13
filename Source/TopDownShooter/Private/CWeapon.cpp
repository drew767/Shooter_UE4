// Fill out your copyright notice in the Description page of Project Settings.


#include "CWeapon.h"
#include "CCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Particles/ParticleSystemComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Runtime\Engine\Public\DrawDebugHelpers.h>
#include "TopDownShooter/TopDownShooter.h"
#include <Math/UnrealMathUtility.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ACWeapon::ACWeapon() :
	m_fireRate(600.0f),
	m_muzzleSocketName("MuzzleSocket"),
	m_muzzleBeginSocketName("MuzzleSocketBegin"),
	m_tracerTargetName("Target"),
	m_baseDamage(20.0f)
{
	m_bulletSpread = 10.0f;
	m_weaponRange = 10000;

	m_meshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
}

FVector ACWeapon::GetMuzzleLocation()
{
	// Case for gamepad to shoot only staright
	return m_meshComponent->GetSocketLocation(m_muzzleSocketName);
}

FRotator ACWeapon::GetMuzzleRotation()
{
	AActor* weaponOwner = GetOwner();
	if (!weaponOwner)
	{
		return FRotator();
	}
	APawn* pawnWeaponOwner = Cast<APawn>(weaponOwner);

	return pawnWeaponOwner->GetViewRotation();
}

FVector ACWeapon::GetCameraDamageStartLocation(const FVector& aimDir) const
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (!owner)
	{
		return FVector::ZeroVector;
	}

	APlayerController* playerController = owner ? Cast<APlayerController>(owner->Controller) : nullptr;
	FVector outStartTrace = FVector::ZeroVector;

	if (playerController)
	{
		FRotator DummyRot;
		playerController->GetPlayerViewPoint(outStartTrace, DummyRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		outStartTrace = outStartTrace + aimDir * (FVector::DotProduct((owner->GetActorLocation() - outStartTrace), aimDir));
	}

	return outStartTrace;
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	m_timeBetweemShots = 60.0f / m_fireRate;
}

void ACWeapon::Fire()
{
	ACCharacter* weaponOwner = Cast<ACCharacter>(GetOwner());

	if (!weaponOwner)
	{
		return;
	}

	// searching camera aim point
	FVector shotDirection = weaponOwner->GetAimingDirection().Vector();
	float halfRad = FMath::DegreesToRadians(m_bulletSpread);
	shotDirection = FMath::VRandCone(shotDirection, halfRad, halfRad);

	FVector cameraLocation = weaponOwner->GetPawnViewLocation();
	FVector shotEndPoint = cameraLocation + (shotDirection * m_weaponRange);
	FVector muzzleLocation = m_meshComponent->GetSocketLocation(m_muzzleBeginSocketName);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(weaponOwner);
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	collisionParams.bReturnPhysicalMaterial = true;
	FHitResult cameraHitResult;
	FHitResult muzzleHitResult;
	FHitResult finalHitResult;
	bool cameraShotHitSomething = GetWorld()->LineTraceSingleByChannel(cameraHitResult, cameraLocation, shotEndPoint, COLLISION_WEAPON, collisionParams);
	bool muzzleShotHitSomething = false;
	bool ignoreMuzzleShot = false;
	if (cameraShotHitSomething) // this check must be performed after camera aiming hit. 
	{
		// checking if character is near the wall and we are aiming to the wall behind character 
		bool ignoreCameraHit = FVector::Dist(cameraLocation, muzzleLocation) > FVector::Dist(cameraLocation, cameraHitResult.ImpactPoint);
		
		if (ignoreCameraHit)
		{
			muzzleShotHitSomething = GetWorld()->LineTraceSingleByChannel(muzzleHitResult, muzzleLocation, shotEndPoint, COLLISION_WEAPON, collisionParams);
			if (muzzleShotHitSomething)
			{
				finalHitResult = muzzleHitResult;
			}
		}
		else
		{
			muzzleShotHitSomething = GetWorld()->LineTraceSingleByChannel(muzzleHitResult, muzzleLocation, cameraHitResult.ImpactPoint, COLLISION_WEAPON, collisionParams);
			if (muzzleShotHitSomething)
			{
				finalHitResult = muzzleHitResult;
			}
			else
			{
				finalHitResult = cameraHitResult;
			}
		}
	}
	else
	{
		muzzleShotHitSomething = GetWorld()->LineTraceSingleByChannel(finalHitResult, muzzleLocation, shotEndPoint, COLLISION_WEAPON, collisionParams);
	}

	if (cameraShotHitSomething || muzzleShotHitSomething)
	{
		//Process damage
		AActor* hitActor = finalHitResult.GetActor();
		EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(finalHitResult.PhysMaterial.Get());

		// APPLY DAMAGE PART SHOULD BE REWORKED
		float damage = m_baseDamage;
		if (surfaceType == SURFACE_FLESHVULNERABLE)
		{
			damage *= 4;
		}
		UGameplayStatics::ApplyPointDamage(hitActor, damage, shotDirection, finalHitResult, weaponOwner->GetInstigatorController(), weaponOwner, m_damageType);

		PlayImpactEffects(surfaceType, finalHitResult.ImpactPoint);

		shotEndPoint = finalHitResult.ImpactPoint;
	}

	PlayFireEffects(shotEndPoint);

	m_lastFireTime = GetWorld()->TimeSeconds;
}

void ACWeapon::AIFire()
{
	ACharacter* weaponOwner = Cast<ACharacter>(GetOwner());

	if (!weaponOwner)
	{
		return;
	}

	FVector muzzleLocation = GetMuzzleLocation();

	// Bullet Spread
	float halfRad = FMath::DegreesToRadians(m_bulletSpread);
	FVector shotDirection = FMath::VRandCone(weaponOwner->GetActorRotation().Vector(), halfRad, halfRad / 3.0f);
	FVector tracerEndPoint = muzzleLocation + (shotDirection * m_weaponRange);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(weaponOwner);
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	collisionParams.bReturnPhysicalMaterial = true;

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, muzzleLocation, tracerEndPoint, COLLISION_WEAPON, collisionParams))
	{
		// Blocking hit! Process damage
		AActor* hitActor = hitResult.GetActor();

		EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());

		float damage = m_baseDamage;
		if (surfaceType == SURFACE_FLESHVULNERABLE)
		{
			damage *= 4;
		}
		UGameplayStatics::ApplyPointDamage(hitActor, damage, shotDirection, hitResult, weaponOwner->GetInstigatorController(), weaponOwner, m_damageType);

		PlayImpactEffects(surfaceType, hitResult.ImpactPoint);

		tracerEndPoint = hitResult.ImpactPoint;
	}

	PlayFireEffects(tracerEndPoint);

	m_lastFireTime = GetWorld()->TimeSeconds;
}

void ACWeapon::StartFire()
{
	float firstDelay = FMath::Max(m_lastFireTime + m_timeBetweemShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(m_timerBetweenShots, this, &ACWeapon::Fire, m_timeBetweemShots, true, firstDelay);
}

void ACWeapon::StartAIFire()
{
	float firstDelay = FMath::Max(m_lastFireTime + m_timeBetweemShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(m_timerBetweenShots, this, &ACWeapon::AIFire, m_timeBetweemShots, true, firstDelay);
}

void ACWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(m_timerBetweenShots);
}

void ACWeapon::PlayImpactEffects(EPhysicalSurface const surfaceType, FVector const& impactPoint)
{
	UParticleSystem* surfaceRelatedEffect = nullptr;
	switch (surfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
	case SURFACE_FLESHLIMB:
	{
		surfaceRelatedEffect = m_valnurableImpactEffect;
		break;
	}
	default:
	{
		surfaceRelatedEffect = m_defaultImpactEffect;
		break;
	}
	}

	if (surfaceRelatedEffect)
	{
		FVector shotDirection = impactPoint - GetMuzzleLocation();
		shotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), surfaceRelatedEffect, impactPoint, shotDirection.Rotation());
	}
}

void ACWeapon::PlayFireEffects(FVector const& traceEnd)
{
	if (m_tracerEffect)
	{
		if (FVector::Dist(traceEnd, GetMuzzleLocation()) > 100.0f)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, m_tracerEffect, m_meshComponent->GetSocketLocation(m_muzzleBeginSocketName), UKismetMathLibrary::FindLookAtRotation(m_meshComponent->GetSocketLocation(m_muzzleBeginSocketName), traceEnd));
			//DrawDebugSphere(GetWorld(), m_meshComponent->GetSocketLocation(m_muzzleBeginSocketName), 10, 5, FColor::Green, false, 0.1f);
		}
	}

	if (m_muzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(m_muzzleEffect, m_meshComponent, m_muzzleSocketName);
	}
}
