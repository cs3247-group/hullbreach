// CircuitPuzzleManager.cpp
#include "CircuitPuzzleManager.h"
#include "TimerManager.h"

ACircuitPuzzleManager::ACircuitPuzzleManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACircuitPuzzleManager::BeginPlay()
{
    Super::BeginPlay();
}

void ACircuitPuzzleManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bGameActive || bGameWon || !ActiveCube.bActive)
        return;

    FallAccumulator += DeltaTime;
    if (FallAccumulator >= FallSpeed)
    {
        FallAccumulator = 0.0f;
        StepFall();
    }
}

void ACircuitPuzzleManager::StartGame()
{
    ResetGame();
    bGameActive = true;
    SpawnNextCube();
}

void ACircuitPuzzleManager::ResetGame()
{
    bGameActive = false;
    bGameWon = false;
    FallAccumulator = 0.0f;

    // Initialize grid
    Grid.Empty();
    Grid.SetNum(GridWidth * GridHeight);

    for (FCircuitCell& Cell : Grid)
    {
        Cell.Reset();
    }

    ActiveCube.bActive = false;
}

// ==================== INPUT ====================

void ACircuitPuzzleManager::MoveLeft()
{
    if (bGameActive && !bGameWon && ActiveCube.bActive)
    {
        Move(-1);
    }
}

void ACircuitPuzzleManager::MoveRight()
{
    if (bGameActive && !bGameWon && ActiveCube.bActive)
    {
        Move(1);
    }
}

void ACircuitPuzzleManager::RotateCube()
{
    if (!bGameActive || bGameWon || !ActiveCube.bActive)
        return;

    ActiveCube.Rotation = (ActiveCube.Rotation + 1) % 4;
    ActiveCube.Ports = ComputePorts(ActiveCube.Type, ActiveCube.Rotation);
}

void ACircuitPuzzleManager::DropCube()
{
    if (!bGameActive || bGameWon || !ActiveCube.bActive)
        return;

    // Drop cube instantly until it hits something
    while (CanPlace(ActiveCube.Coord.X, ActiveCube.Coord.Y - 1))
    {
        ActiveCube.Coord.Y--;
    }

    LockCube();
    SpawnNextCube();
}

// ==================== GRID ACCESS ====================

FCircuitCell ACircuitPuzzleManager::GetCell(int32 X, int32 Y) const
{
    if (IsValidCoord(X, Y))
    {
        return Grid[GetIndex(X, Y)];
    }
    return FCircuitCell();
}

int32 ACircuitPuzzleManager::GetIndex(int32 X, int32 Y) const
{
    return Y * GridWidth + X;
}

bool ACircuitPuzzleManager::IsValidCoord(int32 X, int32 Y) const
{
    return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

bool ACircuitPuzzleManager::CanPlace(int32 X, int32 Y) const
{
    if (!IsValidCoord(X, Y))
        return false;

    return !Grid[GetIndex(X, Y)].bOccupied;
}

// ==================== CUBE LOGIC ====================

EPortMask ACircuitPuzzleManager::ComputePorts(ECubeType Type, uint8 Rotation) const
{
    Rotation %= 4;

    if (Type == ECubeType::Straight)
    {
        // Vertical (N-S) or Horizontal (E-W)
        if (Rotation % 2 == 0)
            return static_cast<EPortMask>((uint8)EPortMask::N | (uint8)EPortMask::S);
        else
            return static_cast<EPortMask>((uint8)EPortMask::E | (uint8)EPortMask::W);
    }
    else // Corner
    {
        switch (Rotation)
        {
            case 0: return static_cast<EPortMask>((uint8)EPortMask::N | (uint8)EPortMask::E);
            case 1: return static_cast<EPortMask>((uint8)EPortMask::E | (uint8)EPortMask::S);
            case 2: return static_cast<EPortMask>((uint8)EPortMask::S | (uint8)EPortMask::W);
            default: return static_cast<EPortMask>((uint8)EPortMask::W | (uint8)EPortMask::N);
        }
    }
}

void ACircuitPuzzleManager::SpawnNextCube()
{
    ActiveCube.bActive = true;
    ActiveCube.Type = FMath::RandBool() ? ECubeType::Straight : ECubeType::Corner;
    ActiveCube.Rotation = 0;
    ActiveCube.Ports = ComputePorts(ActiveCube.Type, ActiveCube.Rotation);
    ActiveCube.Coord = FIntPoint(GridWidth / 2, GridHeight - 1);

    // Check if spawn position is blocked (game over)
    if (!CanPlace(ActiveCube.Coord.X, ActiveCube.Coord.Y))
    {
        bGameActive = false;
        UE_LOG(LogTemp, Warning, TEXT("Game Over: Spawn blocked"));
    }
    else
    {
        OnCubeSpawned();
    }
}

void ACircuitPuzzleManager::StepFall()
{
    int32 NewY = ActiveCube.Coord.Y - 1;

    if (CanPlace(ActiveCube.Coord.X, NewY))
    {
        ActiveCube.Coord.Y = NewY;
    }
    else
    {
        LockCube();
        SpawnNextCube();
    }
}

void ACircuitPuzzleManager::LockCube()
{
    if (!ActiveCube.bActive)
        return;

    int32 X = ActiveCube.Coord.X;
    int32 Y = ActiveCube.Coord.Y;

    if (!IsValidCoord(X, Y))
        return;

    FCircuitCell& Cell = Grid[GetIndex(X, Y)];
    Cell.bOccupied = true;
    Cell.Type = ActiveCube.Type;
    Cell.Rotation = ActiveCube.Rotation;
    Cell.Ports = ActiveCube.Ports;

    ActiveCube.bActive = false;

    OnCubeLocked(X, Y);

    // Recalculate power flow
    ComputePower();
}

void ACircuitPuzzleManager::Move(int32 DX)
{
    int32 NewX = ActiveCube.Coord.X + DX;

    if (CanPlace(NewX, ActiveCube.Coord.Y))
    {
        ActiveCube.Coord.X = NewX;
    }
}

// ==================== CIRCUIT SOLVING (BFS) ====================

void ACircuitPuzzleManager::ComputePower()
{
    // Reset all power states
    for (FCircuitCell& Cell : Grid)
    {
        Cell.bPowered = false;
    }

    // BFS from power node
    TQueue<FIntPoint> Queue;
    TSet<FIntPoint> Visited;

    Queue.Enqueue(PowerNode);
    Visited.Add(PowerNode);

    while (!Queue.IsEmpty())
    {
        FIntPoint Current;
        Queue.Dequeue(Current);

        // Mark as powered
        if (IsValidCoord(Current.X, Current.Y))
        {
            Grid[GetIndex(Current.X, Current.Y)].bPowered = true;
        }

        // Check if we reached the bypass
        if (Current == BypassNode)
        {
            bGameWon = true;
            OnGameWon();
            return;
        }

        // Get current cell
        FCircuitCell& Cell = Grid[GetIndex(Current.X, Current.Y)];
        if (!Cell.bOccupied)
            continue;

        // Check all 4 directions
        TArray<EPortMask> Directions = { EPortMask::N, EPortMask::E, EPortMask::S, EPortMask::W };

        for (EPortMask Dir : Directions)
        {
            // Check if current cell has this port
            if (!EnumHasAnyFlags(Cell.Ports, Dir))
                continue;

            // Get neighbor
            FIntPoint NeighborCoord = GetNeighborCoord(Current, Dir);
            if (!IsValidCoord(NeighborCoord.X, NeighborCoord.Y))
                continue;

            if (Visited.Contains(NeighborCoord))
                continue;

            FCircuitCell& Neighbor = Grid[GetIndex(NeighborCoord.X, NeighborCoord.Y)];
            if (!Neighbor.bOccupied)
                continue;

            // Check if neighbor has the opposite port
            EPortMask OppositeDir = GetOppositePort(Dir);
            if (EnumHasAnyFlags(Neighbor.Ports, OppositeDir))
            {
                Queue.Enqueue(NeighborCoord);
                Visited.Add(NeighborCoord);
            }
        }
    }

    OnPowerChanged();
}

EPortMask ACircuitPuzzleManager::GetOppositePort(EPortMask Port) const
{
    switch (Port)
    {
        case EPortMask::N: return EPortMask::S;
        case EPortMask::S: return EPortMask::N;
        case EPortMask::E: return EPortMask::W;
        case EPortMask::W: return EPortMask::E;
        default: return EPortMask::None;
    }
}

FIntPoint ACircuitPuzzleManager::GetNeighborCoord(FIntPoint Coord, EPortMask Direction) const
{
    switch (Direction)
    {
        case EPortMask::N: return FIntPoint(Coord.X, Coord.Y + 1);
        case EPortMask::S: return FIntPoint(Coord.X, Coord.Y - 1);
        case EPortMask::E: return FIntPoint(Coord.X + 1, Coord.Y);
        case EPortMask::W: return FIntPoint(Coord.X - 1, Coord.Y);
        default: return Coord;
    }
}
