// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyPlayerCharacter::Fire);
	// Only Yaw (left/right turn)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// REMOVE: BindAxis("LookUp", ...) -- don't call pitch

	PlayerInputComponent->BindAction("AltToggle", IE_Pressed, this, &AMyPlayerCharacter::EnableUIInput);
	PlayerInputComponent->BindAction("AltToggle", IE_Released, this, &AMyPlayerCharacter::EnableGameInput);
}

void AMyPlayerCharacter::Fire()
{
	if (FireSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AMyPlayerCharacter::EnableUIInput()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	PC->bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
}

void AMyPlayerCharacter::EnableGameInput()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	PC->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}