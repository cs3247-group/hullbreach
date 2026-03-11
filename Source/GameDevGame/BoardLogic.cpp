// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoardLogic.h"

void UBoardLogic::Init(int32 InW, int32 InH)
{
    W = InW;
    H = InH;

    // Define endpoints in grid coordinates
    StartX = 0;
    StartY = H - 1;      // bottom-left

    EndX = W - 1;
    EndY = H/2;            // top-right

    Grid.SetNum(W * H);
    for (FCell& C : Grid)
    {
        C.Type = ETile::Empty;
    }
}

bool UBoardLogic::IsInside(int32 X, int32 Y) const
{
    return X >= 0 && X < W && Y >= 0 && Y < H;
}

FCell UBoardLogic::GetCell(int32 X, int32 Y) const
{
    if (!IsInside(X, Y)) return FCell();
    return Grid[Y * W + X];
}

bool UBoardLogic::IsEmpty(int32 X, int32 Y) const
{
    return IsInside(X, Y) && GetCell(X, Y).Type == ETile::Empty;
}

bool UBoardLogic::CanOccupy(int32 X, int32 Y) const
{
    if (!IsEmpty(X, Y)) return false;

    // Prevent pieces from occupying start/end cells (recommended)
    if ((X == StartX && Y == StartY) || (X == EndX && Y == EndY))
        return false;

    return true;
}

void UBoardLogic::SpawnNewPiece()
{
    if (bPuzzleSolved)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnNewPiece blocked - puzzle already solved"));
        return;
    }

    Piece.X = W / 2;
    Piece.Y = 0;
    Piece.Type = (FMath::RandBool() ? ETile::Straight : ETile::Cross);

    bHasPiece = true;

    UE_LOG(LogTemp, Warning, TEXT("SpawnNewPiece: Created new piece at (%d,%d)"), Piece.X, Piece.Y);
}

bool UBoardLogic::TryMove(int32 Dx)
{
    if (!bHasPiece) return false;

    const int32 NX = Piece.X + Dx;
    if (CanOccupy(NX, Piece.Y))
    {
        Piece.X = NX;
        return true;
    }
    return false;
}

void UBoardLogic::LockPiece()
{
    if (!bHasPiece) return;

    if (IsInside(Piece.X, Piece.Y))
    {
        FCell& C = Grid[Piece.Y * W + Piece.X];
        C.Type = Piece.Type;
    }

    bHasPiece = false;
}

bool UBoardLogic::StepFall()
{
    if (bPuzzleSolved)
    {
        UE_LOG(LogTemp, Warning, TEXT("StepFall blocked - puzzle solved"));
        return true;
    }

    if (!bHasPiece)
    {
        SpawnNewPiece();

        // Spawn collision => game over
        if (!CanOccupy(Piece.X, Piece.Y)) return false;
        return true;
    }

    const int32 NY = Piece.Y + 1;
    if (CanOccupy(Piece.X, NY))
    {
        Piece.Y = NY;
        return true;
    }

    // Can't fall -> lock
    LockPiece();

    // Check if puzzle is solved after locking
    if (CheckPathExists())
    {
        bPuzzleSolved = true;
        bHasPiece = false;  // Ensure no piece remains
        UE_LOG(LogTemp, Warning, TEXT("Puzzle Solved! Path from Start to End exists!"));
        return true;  // Return immediately, do not spawn new piece
    }

    return true;
}

void UBoardLogic::GetConnectedNeighbors(int32 X, int32 Y, TArray<FIntPoint>& OutNeighbors) const
{
    const FCell Cell = GetCell(X, Y);

    // Special handling for start and end positions - they can connect to any adjacent tile
    const bool bIsStartOrEnd = (X == StartX && Y == StartY) || (X == EndX && Y == EndY);

    if (!bIsStartOrEnd && Cell.Type == ETile::Empty) return;

    // Define which directions this cell connects to based on type and rotation
    TArray<FIntPoint> Directions;

    // All tiles (Start/End, Straight, Cross) connect in all 4 directions
    Directions.Add(FIntPoint(0, -1)); // Up
    Directions.Add(FIntPoint(0, 1));  // Down
    Directions.Add(FIntPoint(-1, 0)); // Left
    Directions.Add(FIntPoint(1, 0));  // Right

    // Check each direction
    for (const FIntPoint& Dir : Directions)
    {
        const int32 NX = X + Dir.X;
        const int32 NY = Y + Dir.Y;

        if (!IsInside(NX, NY)) continue;

        const FCell Neighbor = GetCell(NX, NY);
        const bool bNeighborIsStartOrEnd = (NX == StartX && NY == StartY) || (NX == EndX && NY == EndY);

        // Allow connection to start/end positions, otherwise neighbor must not be empty
        if (!bNeighborIsStartOrEnd && Neighbor.Type == ETile::Empty) continue;

        // If current position is start or end, and neighbor is not empty (or is start/end), allow connection
        if (bIsStartOrEnd)
        {
            OutNeighbors.Add(FIntPoint(NX, NY));
            continue;
        }

        // All tiles connect in all directions, so any non-empty neighbor connects back
        OutNeighbors.Add(FIntPoint(NX, NY));
    }
}

bool UBoardLogic::CheckPathExists()
{
    UE_LOG(LogTemp, Warning, TEXT("CheckPathExists: Starting BFS from (%d,%d) to (%d,%d)"), StartX, StartY, EndX, EndY);

    // BFS from Start to End
    TArray<FIntPoint> Queue;
    TSet<int32> Visited;

    Queue.Add(FIntPoint(StartX, StartY));
    Visited.Add(StartY * W + StartX);

    int32 StepsChecked = 0;

    while (Queue.Num() > 0)
    {
        FIntPoint Current = Queue[0];
        Queue.RemoveAt(0);
        StepsChecked++;

        // Check if we reached the end
        if (Current.X == EndX && Current.Y == EndY)
        {
            UE_LOG(LogTemp, Warning, TEXT("CheckPathExists: PATH FOUND after %d steps!"), StepsChecked);
            return true;
        }

        // Get connected neighbors
        TArray<FIntPoint> Neighbors;
        GetConnectedNeighbors(Current.X, Current.Y, Neighbors);

        // Log cell details
        FCell CellInfo = GetCell(Current.X, Current.Y);
        FString TypeStr = (CellInfo.Type == ETile::Empty) ? TEXT("Empty") : 
                          (CellInfo.Type == ETile::Straight) ? TEXT("Straight") : TEXT("Cross");

        UE_LOG(LogTemp, Warning, TEXT("  Cell (%d,%d): Type=%s, Neighbors=%d"), 
            Current.X, Current.Y, *TypeStr, Neighbors.Num());

        // Log each neighbor
        for (const FIntPoint& Neighbor : Neighbors)
        {
            UE_LOG(LogTemp, Log, TEXT("    -> Neighbor at (%d,%d)"), Neighbor.X, Neighbor.Y);
        }

        for (const FIntPoint& Neighbor : Neighbors)
        {
            const int32 Index = Neighbor.Y * W + Neighbor.X;
            if (!Visited.Contains(Index))
            {
                Visited.Add(Index);
                Queue.Add(Neighbor);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("CheckPathExists: No path found after checking %d cells"), StepsChecked);
    return false;
}