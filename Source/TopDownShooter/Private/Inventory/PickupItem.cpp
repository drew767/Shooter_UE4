// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PickupItem.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Services/CustomGameInstance.h"

// Sets default values
APickupItem::APickupItem()
{
	// Use a sphere as a simple collision representation
	m_collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_collisionComp->InitSphereRadius(5.0f);
	m_collisionComp->SetSimulatePhysics(false);

	// Players can't walk on it
	m_collisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	m_collisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = m_collisionComp;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleItem"));
	m_mesh->SetupAttachment(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	m_uniqueId = UCustomGameInstance::GetInstance()->GetNewObjectID();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

