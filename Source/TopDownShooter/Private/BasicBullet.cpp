// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BasicBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include <Components/StaticMeshComponent.h>

ABasicBullet::ABasicBullet() 
{
	// Use a sphere as a simple collision representation
	m_collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_collisionComp->InitSphereRadius(5.0f);
	m_collisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	m_collisionComp->SetSimulatePhysics(true);
	m_collisionComp->OnComponentHit.AddDynamic(this, &ABasicBullet::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	m_collisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	m_collisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = m_collisionComp;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	m_mesh->SetRelativeScale3D(FVector(0.03f, 0.03f, FMath::RandRange(0.01, 1)));
	m_mesh->SetupAttachment(RootComponent);

	// Use a UProjectileMovementComponent to govern this projectile's movement
	m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	m_projectileMovement->UpdatedComponent = m_collisionComp;
	m_projectileMovement->InitialSpeed = 8000.f;
	m_projectileMovement->MaxSpeed = 8000.f;
	m_projectileMovement->bRotationFollowsVelocity = true;
	m_projectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 0.8f;
}

void ABasicBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	Destroy();
}