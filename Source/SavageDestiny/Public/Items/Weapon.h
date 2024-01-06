// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SAVAGEDESTINY_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator, bool PlayEquippingSound = true);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	FORCEINLINE void DisableMotion() { Amplitude = 0.f; }

protected:

private:
};
