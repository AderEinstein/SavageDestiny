// Copyright: Ader Einstein 2024

#include "Characters/SavageCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/SavageHUD.h"
#include "HUD/SavageOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

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

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASavageCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetEnabledWeaponCollision(ECollisionEnabled::NoCollision); // if character is interrupted when attacking, need to disable its weapon collision

	ActionState = IsAlive() ? EActionState::EAS_HitReacting : EActionState::EAS_Dead;
}

void ASavageCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
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

	InitializeSavageOverlay();

	Tags.Add(FName("EngageableTarget"));
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASavageCharacter::Attack);
	}
}

/* Input Callbacks */

void ASavageCharacter::Move(const FInputActionValue& Value)
{
	if (!CanMove()) return;

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
	if (!CanJump()) return;

	Super::Jump();
}

void ASavageCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeaponRH)
		{
			EquippedWeaponRH->Destroy();
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

void ASavageCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
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

void ASavageCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SavageOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SavageOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASavageCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SavageOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SavageOverlay->SetGold(Attributes->GetGold());
	}
}

float ASavageCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASavageCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->DisableMotion();
	Weapon->DisableOverlapEvents();
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeaponRH = Weapon;
}

void ASavageCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASavageCharacter::DisArm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

bool ASavageCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeaponRH;
}

bool ASavageCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASavageCharacter::CanAttack()
{
	return CanDisArm();
}

bool ASavageCharacter::CanJump()
{
	return (ActionState == EActionState::EAS_Unoccupied || ActionState == EActionState::EAS_Attacking) && ActionState != EActionState::EAS_Dead;
}

bool ASavageCharacter::CanMove()
{
	return ActionState != EActionState::EAS_Dead && ActionState == EActionState::EAS_Unoccupied;
}

void ASavageCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
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
	if (EquippedWeaponRH)
	{
		EquippedWeaponRH->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASavageCharacter::AttachWeaponToHand()
{
	if (EquippedWeaponRH)
	{
		EquippedWeaponRH->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASavageCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASavageCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASavageCharacter::InitializeSavageOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASavageHUD* SavageHUD = Cast<ASavageHUD>(PlayerController->GetHUD());
		if (SavageHUD)
		{
			SavageOverlay = SavageHUD->GetSavageOverlay();
			if (SavageOverlay && Attributes)
			{
				SavageOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SavageOverlay->SetStaminaBarPercent(1.f);
				SavageOverlay->SetGold(0);
				SavageOverlay->SetSouls(0);
			}
		}
	}
}

void ASavageCharacter::SetHUDHealth()
{
	if (SavageOverlay)
	{
		SavageOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
