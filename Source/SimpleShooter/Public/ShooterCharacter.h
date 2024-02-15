// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 600.f;
	
	UPROPERTY(EditAnywhere)
	float RotationRate = 45.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	void Shoot();
	
private:
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);
	void LookUp(const float AxisValue);
	void LookRight(const float AxisValue);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

};