// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class SAVAGEDESTINY_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls = 5;
	float DesiredZ;
	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;
public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 SoulsCount) { Souls = SoulsCount; }
};
