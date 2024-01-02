// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SavageCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SAVAGEDESTINY_API ASavageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASavageCharacter();

	/* Input Callbacks */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	
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

	/* States */
	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
