// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Treasure.h"
#include "BreakableActor.generated.h"

UCLASS()
class SAVAGEDESTINY_API ABreakableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnTreasure();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BreakableRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
	
private:	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	bool bBroken = false;
};
