// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CBasicEnemy.generated.h"

class UCHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class TOPDOWNSHOOTER_API ACBasicEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACBasicEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void UpdatePath();
	FVector GetNextPathPoint();
	void SelfDestruct();
	void DamageSelf();
	void OnCheckNearbyBots();

	UFUNCTION()
		void OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

protected:
	FTimerHandle m_timerHandle_updatePathTimer;
	FTimerHandle m_timerHandle_selfDamage;
	FTimerHandle m_timerHandle_checkPowerLevel;

	FVector m_nextPathPoint;
	float m_timeBetweenUpdates;
	bool m_startedSelfDestruction;
	bool m_exploded;
	int32 m_powerLevel;

	// Dynamic material to pulse on damage
	UMaterialInstanceDynamic* m_dynamicMaterialInstance;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* m_meshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UCHealthComponent* m_healthComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* m_sphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Movement")
		float m_movementForce;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Movement")
		bool m_useVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Movement")
		float m_requiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
		UParticleSystem* m_explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
		float m_explosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
		float m_explosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Destruction")
		float m_selfDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* m_selfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* m_explodeSound;
};
