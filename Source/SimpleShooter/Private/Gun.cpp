// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::IsHit(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector LineStart;
	FRotator LineRotation;
	OwnerController->GetPlayerViewPoint(LineStart, LineRotation);
	ShotDirection = -LineRotation.Vector().GetSafeNormal();

	FVector LineEnd = LineStart + LineRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(TArray<AActor*>{this, GetOwner()});

	return GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn != nullptr);
	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleMesh, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound,  Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool bSucess = IsHit(HitResult, ShotDirection);
	if (!bSucess)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location, ShotDirection.Rotation());
	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *UKismetSystemLibrary::GetDisplayName(HitActor));
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		AController* OwnerController = GetOwnerController();
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
	
}

