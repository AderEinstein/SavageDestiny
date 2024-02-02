// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class SAVAGEDESTINY_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();

protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual void Attack();
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	/* </ABaseCharacter> */

};
