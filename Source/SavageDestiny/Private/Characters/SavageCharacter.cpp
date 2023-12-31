// Copyright: Ader Einstein 2024

#include "Characters/SavageCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapon.h"

ASavageCharacter::ASavageCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	
	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

void ASavageCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterIMC, 0);
		}
	}
}

void ASavageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASavageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Equip);
		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Accelerate);
		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ASavageCharacter::Deccelerate);
	}
}

/* Input Callbacks */

void ASavageCharacter::Move(const FInputActionValue& Value)
{
	if (IsOccupied()) return;

	const FVector2D DirectionValue = Value.Get<FVector2D>();
	if (Controller && DirectionValue != FVector2D::ZeroVector)
	{
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator ControllerYaw = FRotator(0.f, ControllerRotation.Yaw, 0.f);

		const FVector ControllerForwardDirection = FRotationMatrix(ControllerYaw).GetUnitAxis(EAxis::X);
		AddMovementInput(ControllerForwardDirection, DirectionValue.X);

		const FVector ControllerRightDirection = FRotationMatrix(ControllerYaw).GetUnitAxis(EAxis::Y);
		AddMovementInput(ControllerRightDirection, DirectionValue.Y);
	}
}

void ASavageCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (Controller && LookAxisValue != FVector2D::ZeroVector)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASavageCharacter::Jump()
{
	if (IsOccupied()) return;

	Super::Jump();
}

void ASavageCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisArm())
		{
			DisArm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASavageCharacter::Accelerate()
{
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	bAccelerating = true;
}

void ASavageCharacter::Deccelerate()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bAccelerating = false;
}

void ASavageCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASavageCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->DisableMotion();
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ASavageCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

bool ASavageCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void ASavageCharacter::DisArm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

bool ASavageCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void ASavageCharacter::PlayEquipMontage(FName MontageSection)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(MontageSection, EquipMontage);
	}
}

void ASavageCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASavageCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASavageCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}