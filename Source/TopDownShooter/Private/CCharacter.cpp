// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacter.h"
#include <Components/CapsuleComponent.h>
#include <Components/DecalComponent.h>
#include "CWeapon.h"
#include <DrawDebugHelpers.h>
#include <Engine/Engine.h>
#include <GameFramework/Character.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "TopDownShooter/TopDownShooter.h"
#include "CIngameCharacterPlayerController.h"
#include "Camera/CustomSpringArmComponent.h"
#include "Camera/CustomCameraComponent.h"
#include "Inventory/InventoryContainer.h"
#include "Inventory/PlayerInventoryManager.h"
#include "Services/CustomGameInstance.h"
#include "Inventory/PickupItem.h"

// Sets default values
ACCharacter::ACCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_springArmComponent = CreateDefaultSubobject<UCustomSpringArmComponent>(TEXT("SpringArmComponent"));
	m_springArmComponent->SetupAttachment(RootComponent);
	m_springArmComponent->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	m_springArmComponent->bUsePawnControlRotation = true;

	m_cameraComponent = CreateDefaultSubobject<UCustomCameraComponent>(TEXT("CameraComponent"));
	m_cameraComponent->SetupAttachment(m_springArmComponent, USpringArmComponent::SocketName);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACCharacter::OnActorBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACCharacter::OnActorEndOverlap);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
}

void ACCharacter::OnCharacterDeath()
{
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Cast<ACIngameCharacterPlayerController>(GetController())->DetachInputForPlayer();
	DetachFromControllerPendingDestroy();
	StopFire();
	SetLifeSpan(10.0f);
}

FRotator ACCharacter::GetAimingDirection()
{
	APlayerController* pc = Cast<APlayerController>(this->Controller);
	return pc->PlayerCameraManager->GetCameraRotation();
}

void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Cast<ACIngameCharacterPlayerController>(GetController())->SetupInputForPlayer(this);
}

// Called every frame
void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//for (auto* item : m_surroundingCollectibleItems)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("%s %lu"), *(item->GetName()), item->m_modifiableMeta.m_instanceId));
	//	}
	//}

	//Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->GetInventorySystem()->m_characterInventory->Print();
}

void ACCharacter::OnActorBeginOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APickupItem* pickupItem = Cast<APickupItem>(OtherActor);
	if (pickupItem)
	{
		Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->GetInventorySystem()->m_characterInventory->AddItemToInventory(pickupItem);
	}
}

void ACCharacter::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APickupItem* pickupItem = Cast<APickupItem>(OtherActor);
	if (pickupItem)
	{
		//m_surroundingCollectibleItems.Remove(collectible);
	}
}

void ACCharacter::StartFire()
{
	if (m_currentWeapon)
	{
		m_currentWeapon->StartFire();
	}
}

void ACCharacter::StopFire()
{
	if (m_currentWeapon)
	{
		m_currentWeapon->StopFire();
	}
}

FVector ACCharacter::GetPawnViewLocation() const
{
	if (m_cameraComponent)
	{
		return m_cameraComponent->GetComponentLocation();
	}

	return GetPawnViewLocation();
}

void ACCharacter::OnHealthChanged(UCHealthComponent* owningHealthComp, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (health <= 0.0f)
	{
		OnCharacterDeath();
	}
}

// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Spawn a default weapon

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_currentWeapon = GetWorld()->SpawnActor<ACWeapon>(m_starterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	if (m_currentWeapon)
	{
		m_currentWeapon->SetOwner(this);
		m_currentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_weaponHandSocketAttachPoint);
	}
}
