// Copyright Epic Games, Inc. All Rights Reserved.

#include "InputPawn.h"
#include "BoardActor.h"
#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AInputPawn::AInputPawn()
{
    AutoPossessPlayer = EAutoReceiveInput::Player0;
    PrimaryActorTick.bCanEverTick = false;
}

void AInputPawn::BeginPlay()
{
    Super::BeginPlay();

    Board = FindBoard();
    UE_LOG(LogTemp, Warning, TEXT("InputPawn BeginPlay"));

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        UE_LOG(LogTemp, Warning, TEXT("Before Possess: %s"), *GetNameSafe(PC->GetPawn()));
        PC->Possess(this);
        UE_LOG(LogTemp, Warning, TEXT("After Possess: %s"), *GetNameSafe(PC->GetPawn()));

        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;

        EnableInput(PC);
        UE_LOG(LogTemp, Warning, TEXT("EnableInput called"));
    }
}

void AInputPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    UE_LOG(LogTemp, Warning, TEXT("InputPawn PossessedBy called"));
}

ABoardActor* AInputPawn::FindBoard() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    for (TActorIterator<ABoardActor> It(World); It; ++It)
    {
        return *It;
    }
    return nullptr;
}

void AInputPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called"));

    if (!PlayerInputComponent) return;

    PlayerInputComponent->BindKey(EKeys::A, IE_Pressed, this, &AInputPawn::MoveLeft);
    PlayerInputComponent->BindKey(EKeys::D, IE_Pressed, this, &AInputPawn::MoveRight);

    // optional backup keys
    PlayerInputComponent->BindKey(EKeys::Left, IE_Pressed, this, &AInputPawn::MoveLeft);
    PlayerInputComponent->BindKey(EKeys::Right, IE_Pressed, this, &AInputPawn::MoveRight);
}

void AInputPawn::MoveLeft()
{
    UE_LOG(LogTemp, Warning, TEXT("MoveLeft pressed"));

    if (!Board) Board = FindBoard();
    if (Board) Board->InputMove(-1);
    else UE_LOG(LogTemp, Error, TEXT("Board is NULL in MoveLeft"));
}

void AInputPawn::MoveRight()
{
    UE_LOG(LogTemp, Warning, TEXT("MoveRight pressed"));

    if (!Board) Board = FindBoard();
    if (Board) Board->InputMove(1);
    else UE_LOG(LogTemp, Error, TEXT("Board is NULL in MoveRight"));
}