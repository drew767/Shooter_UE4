// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizedCharacterInterface.h"
#include "CBasicAI.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API ACBasicAI : public ACustomizedCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBasicAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StopFire();

	virtual void OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<class ACWeapon> m_starterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName m_weaponAttachSocketName;

	class ACWeapon* m_currentWeapon;
};
