// Fill out your copyright notice in the Description page of Project Settings.

#include "CBasicEnemy.h"
#include <Components/StaticMeshComponent.h>
#include "CCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>
#include <DrawDebugHelpers.h>
#include "CHealthComponent.h"
#include <Components/SphereComponent.h>
#include <Sound/SoundCue.h>

static int32 g_debugTrackerBotDrawing = 0;
FAutoConsoleVariableRef CVARDebugTrackerBotDrawing(TEXT("COOP.DebugTrackerBot"), g_debugTrackerBotDrawing, TEXT("Draw Debug Lines for TrackerBot"), ECVF_Cheat);

// Sets default values
ACBasicEnemy::ACBasicEnemy() :
	m_timeBetweenUpdates(2.0f),
	m_movementForce(1000),
	m_useVelocityChange(false),
	m_requiredDistanceToTarget(100),
	m_explosionRadius(350),
	m_explosionDamage(60),
	m_selfDamageInterval(0.25f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_meshComponent->SetCanEverAffectNavigation(false);
	m_meshComponent->SetSimulatePhysics(true);
	RootComponent = m_meshComponent;

	m_healthComponent = CreateDefaultSubobject<UCHealthComponent>(TEXT("HealthComponent"));
	m_healthComponent->m_onHealthChanged.AddDynamic(this, &ACBasicEnemy::OnHealthChanged);

	m_sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_sphereComponent->SetSphereRadius(200);
	m_sphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_sphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_sphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	m_sphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACBasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(m_timerHandle_checkPowerLevel, this, &ACBasicEnemy::OnCheckNearbyBots, 1.0f, true);
		GetWorldTimerManager().SetTimer(m_timerHandle_updatePathTimer, this, &ACBasicEnemy::UpdatePath, m_timeBetweenUpdates, true, 0.0f);
	}
}

void ACBasicEnemy::UpdatePath()
{
	m_nextPathPoint = GetNextPathPoint();
}

FVector ACBasicEnemy::GetNextPathPoint()
{
	// Hack to get actor location
	//ACharacter* playerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	//UNavigationPath* navPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), playerPawn);

	//if (navPath && navPath->PathPoints.Num() > 1)
	//{
	//	return navPath->PathPoints[1];
	//}
	//////////////////////////////////////////////////////////////////////
	AActor* BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || UCHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		UCHealthComponent* TestPawnHealthComp = Cast<UCHealthComponent>(TestPawn->GetComponentByClass(UCHealthComponent::StaticClass()));
		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();

			if (Distance < NearestTargetDistance)
			{
				BestTarget = TestPawn;
				NearestTargetDistance = Distance;
			}
		}
	}

	if (BestTarget)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);

		GetWorldTimerManager().ClearTimer(m_timerHandle_updatePathTimer);
		GetWorldTimerManager().SetTimer(m_timerHandle_updatePathTimer, this, &ACBasicEnemy::UpdatePath, m_timeBetweenUpdates, false);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
	}
	//////////////////////////////////////////////////////////////////////

	return GetActorLocation();
}

void ACBasicEnemy::OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (m_dynamicMaterialInstance == nullptr)
	{
		m_dynamicMaterialInstance = m_meshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_meshComponent->GetMaterial(0));
	}

	if (m_dynamicMaterialInstance)
	{
		m_dynamicMaterialInstance->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	// Explode on hitpoints == 0
	if (health <= 0.0f)
	{
		SelfDestruct();
	}
}

void ACBasicEnemy::SelfDestruct()
{
	if (m_exploded)
	{
		return;
	}

	m_exploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_explosionEffect, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, m_explodeSound, GetActorLocation());

	m_meshComponent->SetVisibility(false, true);
	m_meshComponent->SetSimulatePhysics(false);
	m_meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		// Increase damage based on the power level (challenge code)
		float ActualDamage = m_explosionDamage + (m_explosionDamage * m_powerLevel);

		// Apply Damage!
		UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), m_explosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		if (g_debugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), m_explosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}
		SetLifeSpan(2.0f);
	}
}

void ACBasicEnemy::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ACBasicEnemy::OnCheckNearbyBots()
{
	// distance to check for nearby bots
	const float radius = 600;

	// Create temporary collision shape for overlaps
	FCollisionShape CollShape;
	CollShape.SetSphere(radius);

	// Only find Pawns (eg. players and AI bots)
	FCollisionObjectQueryParams QueryParams;
	// Our tracker bot's mesh component is set to Physics Body in Blueprint (default profile of physics simulated actors)
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FOverlapResult> overlaps;
	GetWorld()->OverlapMultiByObjectType(overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

	if (g_debugTrackerBotDrawing)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::White, false, 1.0f);
	}

	int32 numberOfBots = 0;
	// loop over the results using a "range based for loop"
	for (FOverlapResult result : overlaps)
	{
		// Check if we overlapped with another tracker bot (ignoring players and other bot types)
		ACBasicEnemy* bot = Cast<ACBasicEnemy>(result.GetActor());
		// Ignore this trackerbot instance
		if (bot && bot != this)
		{
			numberOfBots++;
		}
	}

	const int32 maxPowerLevel = 4;

	// Clamp between min=0 and max=4
	m_powerLevel = FMath::Clamp(numberOfBots, 0, maxPowerLevel);

	// Update the material color
	if (m_dynamicMaterialInstance == nullptr)
	{
		m_dynamicMaterialInstance = m_meshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_meshComponent->GetMaterial(0));
	}
	if (m_dynamicMaterialInstance)
	{
		// Convert to a float between 0 and 1 just like an 'Alpha' value of a texture. Now the material can be set up without having to know the max power level 
		// which can be tweaked many times by gameplay decisions (would mean we need to keep 2 places up to date)
		float Alpha = m_powerLevel / (float)maxPowerLevel;
		// Note: (float)MaxPowerLevel converts the int32 to a float, 
		//	otherwise the following happens when dealing when dividing integers: 1 / 4 = 0 ('PowerLevel' int / 'MaxPowerLevel' int = 0 int)
		//	this is a common programming problem and can be fixed by 'casting' the int (MaxPowerLevel) to a float before dividing.

		m_dynamicMaterialInstance->SetScalarParameterValue("PowerLevelAlpha", Alpha);
	}

	if (g_debugTrackerBotDrawing)
	{
		// Draw on the bot location
		DrawDebugString(GetWorld(), FVector(0, 0, 0), FString::FromInt(m_powerLevel), this, FColor::White, 1.0f, true);
	}
}

void ACBasicEnemy::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	if (!m_startedSelfDestruction && !m_exploded)
	{
		ACCharacter* PlayerPawn = Cast<ACCharacter>(otherActor);
		if (PlayerPawn && !UCHealthComponent::IsFriendly(otherActor, this))
		{
			// We overlapped with a player!

			if (GetLocalRole() == ROLE_Authority)
			{
				// Start self destruction sequence
				GetWorldTimerManager().SetTimer(m_timerHandle_selfDamage, this, &ACBasicEnemy::DamageSelf, m_selfDamageInterval, true, 0.0f);
			}

			m_startedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(m_selfDestructSound, RootComponent);
		}
	}
}

// Called every frame
void ACBasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	float distanceToTarget = (GetActorLocation() - m_nextPathPoint).Size();

	if (distanceToTarget <= m_requiredDistanceToTarget)
	{
		UpdatePath();

		if (g_debugTrackerBotDrawing)
		{
			DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!", nullptr, FColor::Blue, 1.0f);
		}
	}
	else if (!m_exploded)
	{
		FVector forceDirection = m_nextPathPoint - GetActorLocation();
		forceDirection.Normalize();
		forceDirection *= m_movementForce;
		m_meshComponent->AddForce(forceDirection, NAME_None, m_useVelocityChange);

		if (g_debugTrackerBotDrawing)
		{
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + forceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
		}
	}

	if (g_debugTrackerBotDrawing)
	{
		DrawDebugSphere(GetWorld(), m_nextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
	}
}