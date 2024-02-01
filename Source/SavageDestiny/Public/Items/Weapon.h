// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SAVAGEDESTINY_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator, bool PlayEquippingSound = true);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	FORCEINLINE void DisableMotion() { Amplitude = 0.f; }

	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void BoxTrace(FHitResult& BoxHit);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = true;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
