// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class TOPDOWNSHOOTER_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACWeapon();

protected:
	FVector GetMuzzleLocation();
	FRotator GetMuzzleRotation();
	FVector GetCameraDamageStartLocation(const FVector& aimDir) const;

	virtual void BeginPlay() override;

	void PlayFireEffects(FVector const& traceEnd);
	virtual void Fire();
	void AIFire();

public:
	void StartFire();
	void StartAIFire();
	void StopFire();

private:
	void PlayImpactEffects(EPhysicalSurface const surfaceType, FVector const& impactPoint);

protected:
	FTimerHandle m_timerBetweenShots;
	float m_lastFireTime;
	float m_timeBetweemShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float m_fireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* m_meshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> m_damageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* m_muzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* m_defaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* m_valnurableImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* m_tracerEffect;	

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float m_bulletSpread;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName m_muzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName m_muzzleBeginSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName m_tracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float m_baseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float m_weaponRange;
};
