// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyAlertWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UEnemyAlertWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AlertText;

    void SetAlertText(const FString& Message)
    {
        if (AlertText)
        {
            AlertText->SetText(FText::FromString(Message));
        }
    }

    void ShowAlert()
    {
        if (AlertText)
        {
            AlertText->SetVisibility(ESlateVisibility::Visible);
        }
    }

    void HideAlert()
    {
        if (AlertText)
        {
            AlertText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ColorText;

    void SetColorText(const FString& Message)
    {
        if (ColorText)
        {
            ColorText->SetText(FText::FromString(Message));
        }
    }

    void ShowColor()
    {
        if (ColorText)
        {
            ColorText->SetVisibility(ESlateVisibility::Visible);
        }
    }

    void HideColor()
    {
        if (ColorText)
        {
            ColorText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
};