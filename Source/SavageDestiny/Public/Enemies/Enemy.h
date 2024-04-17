// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;
class ASoul;
/**
 * 
 */
UCLASS()
class SAVAGEDESTINY_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();

	/* <IHitInterface> */
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* </IHitInterface> */

protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual void Attack();
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void Die_Implementation() override;
	/* </ABaseCharacter> */

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState;

private:
	/* AI Behaviour */
	void CheckPatrolTarget();
	void StartPatrolling();
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	void CheckCombatTarget();
	void StartAttackTimer();
	void ChaseTarget();
	void LoseInterest();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	void ClearPatrolTimer();
	void ClearAttackTimer();

	bool InTargetRange(AActor* actor, double Radius);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();

	void SpawnSoul();
	
	/* Health Widget */
	void ShowHealthBar();
	void HideHealthBar();

	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 130.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 750.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceRadius = 50.f;

	
	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = "Death")
	TSubclassOf<ASoul> SoulClass;

};
