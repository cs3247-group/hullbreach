// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BoardLogic.generated.h"

UENUM(BlueprintType)
enum class ETile : uint8
{
    Empty = 0,
    Straight,
    Cross
};

USTRUCT(BlueprintType)
struct FCell
{
    GENERATED_BODY()

    UPROPERTY() ETile Type = ETile::Empty;
};

USTRUCT(BlueprintType)
struct FFallingPiece
{
    GENERATED_BODY()

    UPROPERTY() int32 X = 0;
    UPROPERTY() int32 Y = 0;
    UPROPERTY() ETile Type = ETile::Straight;
};

UCLASS()
class GAMEDEVGAME_API UBoardLogic : public UObject
{
    GENERATED_BODY()

public:
    void Init(int32 InW, int32 InH);

    void SpawnNewPiece();
    bool TryMove(int32 Dx);
    // Returns false if cannot spawn new piece (game over condition)
    bool StepFall();

    int32 GetW() const { return W; }
    int32 GetH() const { return H; }

    FCell GetCell(int32 X, int32 Y) const;

    FFallingPiece GetPiece() const { return Piece; }
    bool HasPiece() const { return bHasPiece; }

    int32 GetStartX() const { return StartX; }
    int32 GetStartY() const { return StartY; }
    int32 GetEndX() const { return EndX; }
    int32 GetEndY() const { return EndY; }

    bool IsPuzzleSolved() const { return bPuzzleSolved; }
    bool CheckPathExists();

private:
    bool IsInside(int32 X, int32 Y) const;
    bool IsEmpty(int32 X, int32 Y) const;
    bool CanOccupy(int32 X, int32 Y) const;
    void LockPiece();
    void GetConnectedNeighbors(int32 X, int32 Y, TArray<FIntPoint>& OutNeighbors) const;

private:
    int32 W = 0;
    int32 H = 0;

    // Updated: grid stores both tile type and rotation
    TArray<FCell> Grid; // W*H

    int32 StartX = 0;
    int32 StartY = 0;
    int32 EndX = 0;
    int32 EndY = 0;

    bool bHasPiece = false;
    FFallingPiece Piece;

    bool bPuzzleSolved = false;
};