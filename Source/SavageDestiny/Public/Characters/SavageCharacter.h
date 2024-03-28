// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "BaseCharacter.h"
#include "SavageCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AItem;
class AWeapon;
class ATreasure;
class ASoul;
class USavageOverlay;

UCLASS()
class SAVAGEDESTINY_API ASavageCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASavageCharacter();
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void Die_Implementation() override;

	/* Input Callbacks */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	void Attack() override; 
	void ResetAttackTrigger();
	void Jump() override;
	void Accelerate();
	void Deccelerate();

	/* Combat */
	void EquipWeapon(AWeapon* Weapon);
	void Arm();
	void DisArm();
	bool CanArm();
	bool CanDisArm();
	bool CanAttack() override;
	bool CanJump();
	bool CanMove();

	void AttackMinEnd();
	void AttackEnd() override;
	int32 AttacksExecuted = 0;

	virtual void SetOverlappingItem(AItem* Item) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;

	void PlayEquipMontage(FName MontageSection);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitializeSavageOverlay();
	void SetHUDHealth();

	/* Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	/* Enhanced Input */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* CharacterIMC;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AccelerateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Input)
	UInputAction* AttackAction;

	/* States */
	UPROPERTY(VisibleAnywhere, Category = State)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = State)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bAccelerating = false;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bAttackRequested = false;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bCanAttack = true;

	/* Items */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* EquipMontage;

	/* Widget */
	UPROPERTY()
	USavageOverlay* SavageOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE bool IsOccupied() const { return ActionState != EActionState::EAS_Unoccupied; }
	FORCEINLINE bool IsAccelerating() const { return bAccelerating; }
};
