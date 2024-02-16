// Copyright: Ader Einstein 2024


#include "Enemies/Enemy.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Soul.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	EnemyState = EEnemyState::EES_NoState;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	// AI Navigation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget) HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	HideHealthBar();
	EnemyController = Cast<AAIController>(GetController());
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ShowHealthBar();
}

void AEnemy::Die_Implementation()
{
	if (IsDead()) return; // die only once
	Super::Die_Implementation();
	EnemyState = EEnemyState::EES_Dead;
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	SpawnSoul();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

void AEnemy::Attack()
{
	Super::Attack();
}

bool AEnemy::CanAttack()
{
	return true;
}

void AEnemy::AttackEnd()
{
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}


void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::StartPatrolling, WaitTime);
	}
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		LoseInterest();
		ClearAttackTimer();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::StartAttackTimer()
{
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState == EEnemyState::EES_Patrolling && SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		ClearPatrolTimer();
		CombatTarget = SeenPawn;
		ChaseTarget();
	}
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (!Target) return false;
	double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->SetOwner(this);
		}
	}
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);	
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

