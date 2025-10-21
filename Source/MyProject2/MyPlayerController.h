// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAlertWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"   // NEWLY ADDED
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void ToggleView();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> ToggleViewWidgetClass;

    UPROPERTY()
    UEnemyAlertWidget* ViewWidgetInstance;


protected:
    virtual void BeginPlay() override;

private:
    AActor* LastViewTarget = nullptr;
    bool bIsEnemyView = false;
};
