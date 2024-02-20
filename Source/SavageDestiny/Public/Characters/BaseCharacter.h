// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class SAVAGEDESTINY_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// Combat
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void HandleDamage(float DamageAmount);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	void DisableCapsule();
	bool IsAlive();
	virtual bool CanAttack();

	// Montage functions
	int32 PlayAttackMontage();
	void StopAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	int32 PlayDeathMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void SetEnabledWeaponCollision(ECollisionEnabled::Type CollisionEnabled, const FName& Weapon = FName("RH"));
	void SetEnabledWeaponCollision(ECollisionEnabled::Type CollisionEnabled, AWeapon* Weapon);
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> WeaponClassRH;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> WeaponClassLH;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* EquippedWeaponRH;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* EquippedWeaponLH;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

private:
	// Utils
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> HitReactMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;
};
