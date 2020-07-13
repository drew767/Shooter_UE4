// Fill out your copyright notice in the Description page of Project Settings.

#include "CHealthComponent.h"
#include <Math/UnrealMathUtility.h>
#include <GameFramework/Actor.h>
#include "CRaidGameModeBase.h"

// Sets default values for this component's properties
UCHealthComponent::UCHealthComponent()
{
	m_defaultHealth = 100;
	m_isDead = false;

	m_teamNum = 255;
}


// Called when the game starts
void UCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();
	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UCHealthComponent::HandleTakeAnyDamage);
	}

	m_healthPoints = m_defaultHealth;
}

bool UCHealthComponent::IsDead()
{
	return m_isDead;
}

void UCHealthComponent::HandleTakeAnyDamage(AActor* damagedActor, float damage, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (damage <= 0.0f || m_isDead)
	{
		return;
	}

	if (damageCauser != damagedActor && IsFriendly(damagedActor, damageCauser))
	{
		return;
	}

	// Update health clamped
	m_healthPoints = FMath::Clamp(m_healthPoints - damage, 0.0f, m_defaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(m_healthPoints));

	m_isDead = m_healthPoints <= 0.0f;

	m_onHealthChanged.Broadcast(this, m_healthPoints, damage, damageType, instigatedBy, damageCauser);

	if (m_isDead)
	{
		ACRaidGameModeBase* gameMode = Cast<ACRaidGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->m_onActorKilled.Broadcast(GetOwner(), damageCauser, instigatedBy);
		}
	}
}

bool UCHealthComponent::IsFriendly(AActor* actorA, AActor* actorB)
{
	if (!actorA || !actorB)
	{
		// Assume Friendly
		return true;
	}

	UCHealthComponent* healthCompA = Cast<UCHealthComponent>(actorA->GetComponentByClass(UCHealthComponent::StaticClass()));
	UCHealthComponent* healthCompB = Cast<UCHealthComponent>(actorB->GetComponentByClass(UCHealthComponent::StaticClass()));

	if (!healthCompA || !healthCompB)
	{
		return true;
	}

	return healthCompA->m_teamNum == healthCompB->m_teamNum;
}

float UCHealthComponent::GetHealth() const
{
	return m_healthPoints;
}
