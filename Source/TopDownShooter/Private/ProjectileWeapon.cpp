// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include <Engine/World.h>
#include "GameFramework/Controller.h"
#include "BasicBullet.h"
#include "CCharacter.h"
#include <DrawDebugHelpers.h>

void AProjectileWeapon::Fire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();

		ACharacter* weaponOwner = Cast<ACharacter>(GetOwner());
		const FRotator SpawnRotation = weaponOwner->GetActorRotation();
		
		FVector SpawnLocation =  GetMuzzleLocation();

		DrawDebugSphere(GetWorld(), SpawnLocation, 10, 15, FColor::Blue);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		float halfRad = FMath::DegreesToRadians(15);
		FVector shotDirection = FMath::VRandCone(SpawnRotation.Vector(), halfRad, halfRad / 3.0f);

		// spawn the projectile at the muzzle
		World->SpawnActor<ABasicBullet>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		//for (int i = 0; i < 7; i++)
		//{
		//	SpawnLocation.X += 50;
		//	for (int j = 0; j < 7; j++)
		//	{
		//		SpawnLocation.Y += 50;
		//		for (int m = 0; m < 7; m++)
		//		{
		//			SpawnLocation.Z += 50;
		//		}
		//	}
		//}
	}

	//// try and play the sound if specified
	//if (FireSound != NULL)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}

	//// try and play a firing animation if specified
	//if (FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}
}
