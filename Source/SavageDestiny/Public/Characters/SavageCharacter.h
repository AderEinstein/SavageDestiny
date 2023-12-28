// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SavageCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

UCLASS()
class SAVAGEDESTINY_API ASavageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASavageCharacter();

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
};
