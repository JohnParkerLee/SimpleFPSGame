// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"

#include "FPSCharacter.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
 
	//创建静态网格体，并将其附着在根组件上
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatciMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetupAttachment(StaticMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetSimulatePhysics(false);
	
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
	AFPSCharacter* Picker = Cast<AFPSCharacter>(OtherActor);
	if(Picker)
	{
		if(!Picker->bIsCarryWeapon)
		{
			AttachToComponent(Picker->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
			Picker->EquidWeapon = this;
			Picker->bIsCarryWeapon = true;
		}else
		{
			Picker->EquidWeapon->Destroy();
			AttachToComponent(Picker->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
			Picker->EquidWeapon = this;
			Picker->bIsCarryWeapon = true;
		}
		
	}
	
	//Picker->GetMesh1P()->SetupAttachment(nullptr, "GripPoint");
	//Picker->GetMesh1P()
	//Picker->GetMesh1P()->GetAttachChildren()->
	//WeaponMesh->SetupAttachment(Picker->GetMesh1P(), "GripPoint");
	//GunMeshComponent->CastShadow = false;
	//GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
	//GunMeshComponent->SetupAttachment(
	//Picker->GetMesh1P()->SetupAttachment()
	
}


