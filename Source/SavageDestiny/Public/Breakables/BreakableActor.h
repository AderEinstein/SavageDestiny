// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Items/Treasure.h"
#include "BreakableActor.generated.h"

UCLASS()
class SAVAGEDESTINY_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnTreasure();

	UFUNCTION(BlueprintCallable)
	void PlayBreakableSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BreakableRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
	
private:	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	USoundBase* BreakableSound;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	float BreakableLifespan = 5.f;
};
