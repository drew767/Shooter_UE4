// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownShooter/TopDownShooter.h"
#include "CustomizedCharacterInterface.h"
#include "CCharacter.generated.h"

class UCHealthComponent;
class UCustomCameraComponent;
class UCustomSpringArmComponent;
class ACWeapon;
class UPrimitiveComponent;
class ACollectiblePhysicWorldItem;

UCLASS()
class TOPDOWNSHOOTER_API ACCharacter : public ACustomizedCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacter();
	void StartFire();
	void StopFire();

	virtual void OnCharacterDeath() override;

	virtual FVector GetPawnViewLocation() const override;
	FRotator GetAimingDirection();

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	virtual void OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCustomCameraComponent* m_cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCustomSpringArmComponent* m_springArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACWeapon> m_starterWeaponClass;

	ACWeapon* m_currentWeapon;
};
