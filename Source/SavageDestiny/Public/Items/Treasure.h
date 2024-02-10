// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

UCLASS()
class SAVAGEDESTINY_API ATreasure : public AItem
{
	GENERATED_BODY()
protected:
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
