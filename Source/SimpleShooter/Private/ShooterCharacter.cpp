// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"


#include "AIController.h"
#include "Gun.h"
#include "SimpleShooterGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeLocation(FVector(0.f, 40.f, 70.f));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SocketOffset = FVector(0.f, 60.f, 0.f);
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("HP : %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		check(GameMode != nullptr);
		
		GameMode->PawnKilled(this);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AAIController* AIController = Cast<AAIController>(EventInstigator);
		if (AIController != nullptr)
		{
			AIController->GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
			AIController->GetBlackboardComponent()->ClearValue(TEXT("FoundPlayerLocation"));
		}
	}

	return ActualDamage;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return (MaxHealth <= 0.f) ? 0.f : (Health / MaxHealth);
}

void AShooterCharacter::MoveForward(const float AxisValue)
{
	const float ScaleValue = AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds();
	AddMovementInput(GetActorForwardVector(), ScaleValue);
}

void AShooterCharacter::MoveRight(const float AxisValue)
{
	const float ScaleValue = AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds();
	AddMovementInput(GetActorRightVector(), ScaleValue);
}

void AShooterCharacter::LookUp(const float AxisValue)
{
	const float ScaleValue = AxisValue * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(ScaleValue);
}

void AShooterCharacter::LookRight(const float AxisValue)
{
	const float ScaleValue = AxisValue * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(ScaleValue);
}

void AShooterCharacter::Shoot()
{
	// 장비가 없는 AGun 클래스가 존재함
	check(Gun != nullptr)

	Gun->PullTrigger();
}



