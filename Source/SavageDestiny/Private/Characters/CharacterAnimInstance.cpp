// Copyright: Ader Einstein 2024

#include "Characters/CharacterAnimInstance.h"
#include "Characters/SavageCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SavageCharacter = Cast<ASavageCharacter>(TryGetPawnOwner());
	if (SavageCharacter)
	{
		SavageCharacterMovement = SavageCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SavageCharacter)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SavageCharacterMovement->Velocity);
		IsFalling = SavageCharacterMovement->IsFalling();
		IsAccelerating = SavageCharacter->IsAccelerating();
		CharacterState = SavageCharacter->GetCharacterState();
	}
}
