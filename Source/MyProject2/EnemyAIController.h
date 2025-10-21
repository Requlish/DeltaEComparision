// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();
    void SetPlayerInView(bool bInView); // Called from EnemyCharacter

    void MoveToRandomLocation();

protected:
    virtual void BeginPlay() override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


    FTimerHandle PatrolTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float PatrolRadius = 1000.f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float WaitTimeBetweenMoves = 2.0f;

private:
    bool bIsPlayerInView = false;

};
