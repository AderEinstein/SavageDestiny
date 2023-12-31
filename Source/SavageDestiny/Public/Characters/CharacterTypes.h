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
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EAS_Dead UMETA(DisplayName = "Dead")
};