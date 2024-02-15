// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
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

	EEnemyState EnemyState;

private:
	/* AI Behaviour */
	bool IsDead();
	void SpawnSoul();

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<ASoul> SoulClass;

	/* Health Widget */
	void ShowHealthBar();
	void HideHealthBar();
	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarWidget;
};
