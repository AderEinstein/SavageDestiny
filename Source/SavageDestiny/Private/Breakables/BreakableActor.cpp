// Copyright: Ader Einstein 2024

#include "Breakables/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BreakableRoot = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Collection Root"));
	RootComponent = BreakableRoot;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(GetRootComponent());
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GeometryCollection);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	PlayBreakableSound();
	SpawnTreasure();
	SetLifeSpan(BreakableLifespan);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}


void ABreakableActor::SpawnTreasure()
{
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		if (TreasureClasses[Selection])
		{
			World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
		}
	}
}

void ABreakableActor::PlayBreakableSound()
{
	if (BreakableSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			BreakableSound,
			GetActorLocation()
		);
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}


