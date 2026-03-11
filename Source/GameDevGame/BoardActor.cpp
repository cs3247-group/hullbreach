// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardActor.h"
#include "CubeBlockActor.h"
#include "Engine/World.h"

ABoardActor::ABoardActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABoardActor::BeginPlay()
{
    Super::BeginPlay();

    Logic = NewObject<UBoardLogic>(this);
    Logic->Init(Width, Height);

    PrevGrid.SetNum(Width * Height);
    for (ETile& T : PrevGrid) T = ETile::Empty;

    SpawnStartEndMarkers();

    EnsureActiveCube();
    // Spawn initial piece so you see something immediately
    Logic->StepFall();
    RefreshActiveCubeVisual();
}

void ABoardActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bGameOver || (Logic && Logic->IsPuzzleSolved())) return;

    Accum += DeltaSeconds;
    if (Accum >= FallInterval)
    {
        Accum -= FallInterval;

        // before step: remember grid
        // (we will compare after to spawn locked cubes)
        // PrevGrid already holds last state

        const bool ok = Logic->StepFall();
        if (!ok)
        {
            bGameOver = true;
            if (ActiveCube) ActiveCube->SetActorHiddenInGame(true);
            return;
        }

        ApplyLockSpawnsIfAny();
        RefreshActiveCubeVisual();

        // Check if puzzle is solved and hide active cube
        if (Logic && Logic->IsPuzzleSolved())
        {
            if (ActiveCube)
            {
                ActiveCube->SetActorHiddenInGame(true);
            }
            UE_LOG(LogTemp, Warning, TEXT("Board: Puzzle completed, stopping game!"));
        }
    }
}

FVector ABoardActor::CellToWorld(int32 X, int32 Y) const
{
    // Board origin is actor location; Y increases downward in grid -> map to -Y in world if you want.
    const FVector Origin = GetActorLocation();
    return Origin + FVector(X * CellSize, 0.f, -Y * CellSize);
}

void ABoardActor::EnsureActiveCube()
{
    if (ActiveCube || !CubeClass) return;
    if (Logic && Logic->IsPuzzleSolved()) return;  // Don't create cube if puzzle is solved

    FActorSpawnParameters Params;
    Params.Owner = this;
    ActiveCube = GetWorld()->SpawnActor<ACubeBlockActor>(CubeClass, GetActorLocation(), FRotator::ZeroRotator, Params);
}

void ABoardActor::RefreshActiveCubeVisual()
{
    if (Logic && Logic->IsPuzzleSolved())
    {
        // Hide and don't refresh if puzzle is solved
        if (ActiveCube)
        {
            ActiveCube->SetActorHiddenInGame(true);
        }
        return;
    }

    EnsureActiveCube();
    if (!ActiveCube) return;

    if (!Logic->HasPiece())
    {
        ActiveCube->SetActorHiddenInGame(true);
        return;
    }

    const FFallingPiece P = Logic->GetPiece();
    ActiveCube->SetActorHiddenInGame(false);
    ActiveCube->SetActorLocation(CellToWorld(P.X, P.Y));
    ActiveCube->SetType(P.Type);
}

void ABoardActor::SpawnLockedCubeAt(int32 X, int32 Y, ETile Type)
{
    if (!CubeClass) return;

    FActorSpawnParameters Params;
    Params.Owner = this;

    ACubeBlockActor* Block = GetWorld()->SpawnActor<ACubeBlockActor>(
        CubeClass,
        CellToWorld(X, Y),
        FRotator::ZeroRotator,
        Params
    );
    if (Block)
    {
        Block->SetType(Type);
    }
}

void ABoardActor::ApplyLockSpawnsIfAny()
{
    // Compare current grid to PrevGrid; spawn actors for any cell that changed from Empty to non-empty
    for (int32 Y = 0; Y < Height; Y++)
    {
        for (int32 X = 0; X < Width; X++)
        {
            const int32 Idx = Y * Width + X;
            const ETile Now = Logic->GetCell(X, Y).Type;
            const ETile Before = PrevGrid[Idx];

            if (Before == ETile::Empty && Now != ETile::Empty)
            {
                SpawnLockedCubeAt(X, Y, Now);
            }

            PrevGrid[Idx] = Now;
        }
    }
}

void ABoardActor::SpawnStartEndMarkers()
{
    if (!Logic || !CubeClass) return;

    FActorSpawnParameters Params;
    Params.Owner = this;

    // Start marker at bottom-left
    StartMarker = GetWorld()->SpawnActor<ACubeBlockActor>(
        CubeClass,
        CellToWorld(Logic->GetStartX(), Logic->GetStartY()) + FVector(0.f, 0.f, 20.f),
        FRotator::ZeroRotator,
        Params
    );

    if (StartMarker)
    {
        StartMarker->SetType(ETile::Cross);
    }

    // End marker at top-right
    EndMarker = GetWorld()->SpawnActor<ACubeBlockActor>(
        CubeClass,
        CellToWorld(Logic->GetEndX(), Logic->GetEndY()) + FVector(0.f, 0.f, 20.f),
        FRotator::ZeroRotator,
        Params
    );

    if (EndMarker)
    {
        EndMarker->SetType(ETile::Cross);
    }
}

void ABoardActor::InputMove(int32 Dx)
{
    UE_LOG(LogTemp, Warning, TEXT("BoardActor InputMove Dx=%d"), Dx);

    if (bGameOver || !Logic || Logic->IsPuzzleSolved()) return;

    const FFallingPiece Before = Logic->GetPiece();
    const bool bMoved = Logic->TryMove(Dx);
    const FFallingPiece After = Logic->GetPiece();

    UE_LOG(LogTemp, Warning, TEXT("Moved=%d  Before=(%d,%d) After=(%d,%d)"),
        bMoved,
        Before.X, Before.Y,
        After.X, After.Y);

    if (bMoved)
    {
        RefreshActiveCubeVisual();
    }
}
