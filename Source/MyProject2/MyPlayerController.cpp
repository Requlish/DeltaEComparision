// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAICharacter.h"
#include "EngineUtils.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Store original player view
    LastViewTarget = GetPawn();

    // Show the Toggle View UI if a widget class is assigned
    if (ToggleViewWidgetClass)
    {
        UUserWidget* ToggleViewWidget = CreateWidget<UUserWidget>(this, ToggleViewWidgetClass);
        if (ToggleViewWidget)
        {
            ViewWidgetInstance = CreateWidget<UEnemyAlertWidget>(this, ToggleViewWidgetClass);
            if (ViewWidgetInstance)
            {
                ViewWidgetInstance->AddToViewport();
                ViewWidgetInstance->HideAlert();
                ViewWidgetInstance->HideColor();

                bShowMouseCursor = true;

                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(ViewWidgetInstance->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                SetInputMode(InputMode);
            }
        }
    }

}

void AMyPlayerController::ToggleView()
{
    if (bIsEnemyView)
    {
        SetViewTargetWithBlend(LastViewTarget, 0.5f);
        bIsEnemyView = false;
    }
    else
    {
        for (TActorIterator<AEnemyAICharacter> It(GetWorld()); It; ++It)
        {
            AEnemyAICharacter* Enemy = *It;
            if (Enemy && Enemy->EnemyCamera)
            {
                SetViewTargetWithBlend(Enemy, 0.5f);
                bIsEnemyView = true;
                break;
            }
        }
    }
}
