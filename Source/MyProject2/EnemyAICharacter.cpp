// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICharacter.h"
#include "EnemyAIController.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h" // Debugging Camera
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Engine/Scene.h" // For FPostProcessSettings
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h" // Color Sampling
#include "Math/Color.h" // Delta E 76
#include "Math/Vector.h" // Delta E 76
#include "Math/Matrix.h" // Delta E 76

namespace {
    // For Delta E values

    struct FLabColor {
        float L, a, b;
    };

    FLabColor XYZToLab(float X, float Y, float Z) {
        const float Xn = 0.95047f;
        const float Yn = 1.00000f;
        const float Zn = 1.08883f;

        auto f = [](float t) {
            return (t > 0.008856f) ? FMath::Pow(t, 1.0f / 3.0f) : (7.787f * t + 16.0f / 116.0f);
            };

        float fx = f(X / Xn);
        float fy = f(Y / Yn);
        float fz = f(Z / Zn);

        return {
            116.0f * fy - 16.0f,
            500.0f * (fx - fy),
            200.0f * (fy - fz)
        };
    }

    float GammaToLinear(float channel)
    {
        return (channel <= 0.04045f)
            ? (channel / 12.92f)
            : FMath::Pow((channel + 0.055f) / 1.055f, 2.4f);
    }

    FLabColor LinearColorToLab(const FLinearColor &Color)
    {
        float R = GammaToLinear(FMath::Clamp(Color.R, 0.f, 1.f));
        float G = GammaToLinear(FMath::Clamp(Color.G, 0.f, 1.f));
        float B = GammaToLinear(FMath::Clamp(Color.B, 0.f, 1.f));

        // Convert to XYZ using D65 reference white
        float X = R * 0.4124f + G * 0.3576f + B * 0.1805f;
        float Y = R * 0.2126f + G * 0.7152f + B * 0.0722f;
        float Z = R * 0.0193f + G * 0.1192f + B * 0.9505f;

        return XYZToLab(X, Y, Z);
    }

    float DeltaE(const FLabColor& Lab1, const FLabColor& Lab2) {
        return FMath::Sqrt(
            FMath::Square(Lab1.L - Lab2.L) +
            FMath::Square(Lab1.a - Lab2.a) +
            FMath::Square(Lab1.b - Lab2.b)
        );
    }

    // Usage example:
    float ComputeColorDifference(const FLinearColor& A, const FLinearColor& B) {
        return DeltaE(LinearColorToLab(A), LinearColorToLab(B));
    }

}

// Sets default values
AEnemyAICharacter::AEnemyAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EnemyCamera"));
	EnemyCamera->SetupAttachment(GetMesh()); // Optional: attach to head bone if skeletal mesh has one
	EnemyCamera->SetRelativeLocation(FVector(0.f, 0.f, 80.f)); // Adjust based on mesh height
	EnemyCamera->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f)); // Look slightly downward

}

// Called when the game starts or when spawned
void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	// Optional: Lock rotation to Yaw only if needed
	// bUseControllerRotationYaw = true;
	// GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

}

// Called every frame
void AEnemyAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());

    // Check if camera exists
    if (!EnemyCamera) return;

    // Get player
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector CameraLocation = EnemyCamera->GetComponentLocation();
    FVector CameraForward = EnemyCamera->GetForwardVector();
    FVector ToPlayer = PlayerPawn->GetActorLocation() - CameraLocation;
    ToPlayer.Normalize();

    float Dot = FVector::DotProduct(CameraForward, ToPlayer);

    // 0.9 is approx 25 degrees cone of vision
    if (Dot > 0.9f)
    {
        // Optional: Line of sight check (no wall blocking view)
        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit,
            CameraLocation,
            PlayerPawn->GetActorLocation(),
            ECC_Visibility,
            Params
        );

        if (bHit && Hit.GetActor() == PlayerPawn)
        {
            AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            if (PC && PC->ViewWidgetInstance)
            {
                PC->ViewWidgetInstance->SetAlertText("Player In View");
                PC->ViewWidgetInstance->ShowAlert();
            }

            UStaticMeshComponent* TargetMesh = nullptr;
            TArray<UStaticMeshComponent*> MeshComponents;
            PlayerPawn->GetComponents<UStaticMeshComponent>(MeshComponents);

            for (UStaticMeshComponent* Meshhh : MeshComponents)
            {
                if (Meshhh && Meshhh->ComponentHasTag("PlayerBody")) // Tag match
                {
                    TargetMesh = Meshhh;
                    break;
                }
            }

            FLinearColor PlayerColor;
            if (TargetMesh)
            {
                UMaterialInterface* Material = TargetMesh->GetMaterial(0); // Get first material slot
                if (Material)
                {
                    FMaterialParameterInfo ParamInfo("BaseColor"); // Assumes a parameter named "BaseColor"
                    bool bSuccess = Material->GetVectorParameterValue(ParamInfo, PlayerColor);
                }
            }
            
            // ---------- Second trace behind the player ----------

            FVector BehindPlayerTraceEnd = PlayerPawn->GetActorLocation() + ToPlayer * 1000.0f;

            FHitResult HitBehindPlayer;
            FCollisionQueryParams ParamsBehind;
            ParamsBehind.AddIgnoredActor(this);
            ParamsBehind.AddIgnoredActor(PlayerPawn);

            bool bHitBehind = GetWorld()->LineTraceSingleByChannel(
                HitBehindPlayer,
                CameraLocation,
                BehindPlayerTraceEnd,
                ECC_Visibility,
                ParamsBehind
            );


            if (bHitBehind)
            {

                UStaticMeshComponent* MeshComp = HitBehindPlayer.GetActor()->FindComponentByClass<UStaticMeshComponent>();
                if (MeshComp)
                {
                    UMaterialInterface* Material = MeshComp->GetMaterial(0); // Get first material slot
                    if (Material)
                    {
                        FLinearColor Color;
                        FMaterialParameterInfo ParamInfo("BaseColor"); // Assumes a parameter named "BaseColor"
                        bool bSuccess = Material->GetVectorParameterValue(ParamInfo, Color);

                        // Compute Delta E
                        PlayerColor.A = 1.0f;
                        Color.A = 1.0f;
                        float DeltaEValue = ComputeColorDifference(PlayerColor, Color);

                        // Show on UI
                        FString DeltaEText = FString::Printf(TEXT("Delta E: %.0f"), DeltaEValue);
                        PC->ViewWidgetInstance->SetColorText(DeltaEText);
                        PC->ViewWidgetInstance->ShowColor();

                        if (DeltaEValue < 5.0f) {
                            PC->ViewWidgetInstance->SetAlertText("Player Camouflaged, Continuing Patrol");


                            // Resume patrol
                            if (bPlayerInView && AIController)
                            {
                                bPlayerInView = false;
                                AIController->SetPlayerInView(false);
                            }
                        }
                        else {
                            PC->ViewWidgetInstance->SetAlertText("Player Seen, Move out of Vision");

                            bPlayerInView = true;

                            // Stop enemy movement
                            if (AIController)
                            {
                                AIController->SetPlayerInView(true);
                            }
                        }
                    }
                }
            }

            // ---------- End of second trace ----------

        }
        else
        {
            if (bPlayerInView)
            {
                AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
                if (PC && PC->ViewWidgetInstance)
                {
                    PC->ViewWidgetInstance->SetAlertText("Finding Player");
                    PC->ViewWidgetInstance->HideColor();
                }

                bPlayerInView = false;

                // Resume patrol
                if (AIController)
                {
                    AIController->SetPlayerInView(false);
                }
            }
            else {
                AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
                PC->ViewWidgetInstance->SetAlertText("Finding Player");
                PC->ViewWidgetInstance->HideColor();
            }
        }
    }
    else
    {
        if (bPlayerInView)
        {
            AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            if (PC && PC->ViewWidgetInstance)
            {
                PC->ViewWidgetInstance->SetAlertText("Finding Player");
                PC->ViewWidgetInstance->HideColor();
            }

            bPlayerInView = false;

            // Resume patrol
            if (AIController)
            {
                AIController->SetPlayerInView(false);
            }
        }
        else {
            AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            PC->ViewWidgetInstance->SetAlertText("Finding Player");
            PC->ViewWidgetInstance->HideColor();
        }
    }
}

// Called to bind functionality to input
void AEnemyAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
