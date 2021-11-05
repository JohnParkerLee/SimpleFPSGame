// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSObjectiveActor.h"
#include "FPSProjectile.h"
#include "MyGameStateBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "Animation/AnimSequence.h"


AFPSCharacter::AFPSCharacter()
{
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
	
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("SuppleBullet", IE_Pressed, this, &AFPSCharacter::SuppleBullet);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSCharacter::LookUpAtRate);
}


void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole()==ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		EquidWeapon = GetWorld()->SpawnActor<AMyWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
		if (EquidWeapon)
		{
			bIsCarryWeapon = true;
			EquidWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
		}
	}
	//AMyWeapon EquidWeapon= NewObject<>()<AMyWeapon>(this); //必须提供一个 name（CursorToWorld）
	//AMyWeapon->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> FP_GunObj(TEXT("/Game/Weapon/SK_FPGun"));
	//USkeletalMesh* FP_Gun = FP_GunObj.Object;
	//Mesh1PComponent->SetupAttachment(FP_Gun, "GripPoint");
	
	
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (FBulletRemain>0 && bIsCarryWeapon)
	{
		if (EquidWeapon->ProjectileClass)
		{
			// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
			FVector MuzzleLocation = EquidWeapon->WeaponMesh->GetSocketLocation("Muzzle");
			// Use controller rotation which is our view direction in first person
			FRotator MuzzleRotation = GetControlRotation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AFPSProjectile>(EquidWeapon->ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		}
		/*
		if (EquidWeapon->ProjectileClass)
		{
			FCollisionQueryParams CollisionParams;
			ActorLineTraceSingle(OutHit, GetActorLocation(), GetActorForwardVector() * 500.f + GetActorLocation(), ECollisionChannel::ECC_WorldStatic, CollisionParams);
			//OutHit.Location
			FRotator MuzzleRotation= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OutHit.Location);
			UKismetMathLibrary::MakeTransform(EquidWeapon->GetActorLocation(), MuzzleRotation, FVector(1.0,1.0,1.0));
			// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
			FVector MuzzleLocation = EquidWeapon->WeaponMesh->GetSocketLocation("Muzzle");
			// Use controller rotation which is our view direction in first person
			//FRotator MuzzleRotation = GetControlRotation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AFPSProjectile>(EquidWeapon->ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		*/
		// try and play the sound if specified
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
			}
		}
		FBulletRemain-=1;
			
	}
	else if (FBulletRemain==0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::White, TEXT("Press Button to restart"), true, FVector2D(2,2));
	}
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void AFPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AFPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void AFPSCharacter::SuppleBullet()
{
	FBulletRemain = 10;
	AMyGameStateBase *MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	MyGameStateBase->FPoints = 0;
	MyGameStateBase->FPointsSum = 0;
	GEngine->ClearOnScreenDebugMessages();
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

