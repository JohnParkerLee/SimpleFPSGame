// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MyWeapon.generated.h"

class AFPSProjectile;

UCLASS()
class FPSGAME_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh ")
	class UStaticMeshComponent* StaticMesh;
	//重写重叠函数
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	//声明拾取函数
	void Equip(class AMan* Picker);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
