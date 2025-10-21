// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = false; // We don't need tick yet
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Start patrolling
    MoveToRandomLocation();
}

void AEnemyAIController::MoveToRandomLocation()
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem) return;

    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation RandomPoint;

    bool bFound = NavSystem->GetRandomReachablePointInRadius(Origin, PatrolRadius, RandomPoint);

    if (bFound)
    {
        MoveToLocation(RandomPoint.Location);
    }
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!bIsPlayerInView)
    {
        // Wait a bit before moving again
        GetWorld()->GetTimerManager().SetTimer(
            PatrolTimerHandle,
            this,
            &AEnemyAIController::MoveToRandomLocation,
            WaitTimeBetweenMoves,
            false
        );
    }
}

void AEnemyAIController::SetPlayerInView(bool bInView)
{
    bIsPlayerInView = bInView;

    if (bIsPlayerInView)
    {
        StopMovement();
        GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
    }
    else
    {
        // Resume patrol when player is no longer visible
        MoveToRandomLocation();
    }
}
    