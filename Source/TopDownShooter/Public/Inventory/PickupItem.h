// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryEnums.h"
#include "PickupItem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TOPDOWNSHOOTER_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	EInventoryItemId GetItemType() { return m_itemTypeId; }
	uint64 GetIdentifier() { return m_uniqueId; }

protected:
	uint64 m_uniqueId;

	/** Mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
		class UStaticMeshComponent* m_mesh;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup")
		class USphereComponent* m_collisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		EInventoryItemId m_itemTypeId;
};
