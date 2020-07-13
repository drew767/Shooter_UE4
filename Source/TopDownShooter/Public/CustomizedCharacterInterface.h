// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownShooter/TopDownShooter.h"
#include "CustomizedCharacterInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetingStateChangedDelegate, bool, isTargeting);

UCLASS()
class TOPDOWNSHOOTER_API ACustomizedCharacterInterface : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomizedCharacterInterface();
	FName GetInventoryAttachPoint(EInventorySlot Slot) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UFUNCTION()
		virtual void OnHealthChanged(class UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser){};

	virtual void OnCharacterDeath() {};
	void SetTargeting(bool isTargeting);
	void SetIsJumping(bool isJumping);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Jump();
	void OnStartTargeting();
	void OnEndTargeting();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Is player aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
		bool IsTargeting() const;

	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
		FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsInitiatedJump() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FTargetingStateChangedDelegate m_OnTargetingStateChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCHealthComponent* m_healthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName m_weaponHandSocketAttachPoint;

	/* Attachpoint for items carried on the belt/pelvis. */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName m_weaponPrimaryAttachPoint;

	/* Attachpoint for primary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName m_weaponSecondaryAttachPoint;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		bool m_isJumping;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		bool m_isTargeting;
};
