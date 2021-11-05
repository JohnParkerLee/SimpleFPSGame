// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "MyWeapon.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TSubclassOf<AMyWeapon> StarterWeaponClass;
	UPROPERTY(BlueprintReadOnly, Category="Gameplay");
	bool bIsCarryingObjective;
	UPROPERTY(BlueprintReadOnly, Category="Gameplay");
	uint8 FPoints = 0;
	UPROPERTY(BlueprintReadOnly, Category="Gameplay");
	uint8 FBulletRemain = 10;
	bool bIsCarryWeapon = false;
	AMyWeapon* EquidWeapon = nullptr;

protected:
	virtual void BeginPlay() override;
	
	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category="Control")
	void Fire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable, Category="Control")
	void SuppleBullet();
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	FHitResult OutHit;
	
	//AMyWeapon *StartWeapon;
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

