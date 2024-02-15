#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"), // use same action state for unequipping weapon
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_HitReacting UMETA(DisplayName = "Hit Reacting"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "No State"),
	EES_Dead UMETA(DisplayName = "Dead"),
};