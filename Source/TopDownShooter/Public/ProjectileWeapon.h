// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CWeapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AProjectileWeapon : public ACWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void Fire() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABasicBullet> ProjectileClass;

};
