// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardLogic.h"
#include "BoardActor.generated.h"

class ACubeBlockActor;

UCLASS()
class GAMEDEVGAME_API ABoardActor : public AActor
{
    GENERATED_BODY()

public:
    ABoardActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    // Called by input pawn
    void InputMove(int32 Dx);

    // For pawn to find
    UFUNCTION(BlueprintCallable)
    bool IsGameOver() const { return bGameOver; }

    // Grid settings
    UPROPERTY(EditAnywhere, Category="Board")
    int32 Width = 6;

    UPROPERTY(EditAnywhere, Category="Board")
    int32 Height = 10;

    UPROPERTY(EditAnywhere, Category="Board")
    float CellSize = 110.f; // Unreal units

    UPROPERTY(EditAnywhere, Category="Board")
    float FallInterval = 0.6f;

    // Cube actor to spawn
    UPROPERTY(EditDefaultsOnly, Category="Board")
    TSubclassOf<ACubeBlockActor> CubeClass;

private:
    FVector CellToWorld(int32 X, int32 Y) const;

    void EnsureActiveCube();
    void RefreshActiveCubeVisual();
    void SpawnLockedCubeAt(int32 X, int32 Y, ETile Type);
    void ApplyLockSpawnsIfAny(); // detect newly locked cell by scanning differences (simple MVP)
    void SpawnStartEndMarkers();

private:
    UPROPERTY()
    UBoardLogic* Logic;

    UPROPERTY()
    ACubeBlockActor* ActiveCube = nullptr;

    UPROPERTY()
    ACubeBlockActor* StartMarker = nullptr;

    UPROPERTY()
    ACubeBlockActor* EndMarker = nullptr;

    float Accum = 0.f;
    bool bGameOver = false;

    // For simplest lock detection: keep previous grid snapshot
    TArray<ETile> PrevGrid;
};
