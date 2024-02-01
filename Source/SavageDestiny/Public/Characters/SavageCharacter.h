// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SavageCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AItem;
class AWeapon;

UCLASS()
class SAVAGEDESTINY_API ASavageCharacter : public ACharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASavageCharacter();

	/* Input Callbacks */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump() override;
	void Equip();
	void Accelerate();
	void Deccelerate();
	void Attack();

	/* Combat */
	void EquipWeapon(AWeapon* Weapon);
	void Arm();
	bool CanArm();
	void DisArm();
	bool CanDisArm();
	bool CanAttack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void SetOverlappingItem(AItem* Item) override;

	void PlayEquipMontage(FName MontageSection);
	int32 PlayAttackMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void SetEnabledWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	
	// Utils
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

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
	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	bool bAccelerating = false;

	/* Items */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	AWeapon* EquippedWeapon;

	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE bool IsOccupied() const { return ActionState != EActionState::EAS_Unoccupied; }
	FORCEINLINE bool IsAccelerating() const { return bAccelerating; }
};
