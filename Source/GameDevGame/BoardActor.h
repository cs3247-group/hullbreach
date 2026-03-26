// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardLogic.h"
#include "BoardActor.generated.h"

class ACubeBlockActor;
class UBoardTrigger;
class USceneComponent;

UCLASS()
class GAMEDEVGAME_API ABoardActor : public AActor
{
    GENERATED_BODY()

public:
    ABoardActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    // Called by input pawn or directly via input system
    UFUNCTION(BlueprintCallable, Category = "Board")
    void InputMove(int32 Dx);

    // Enable/disable input for this actor (called by trigger or blueprint)
    UFUNCTION(BlueprintCallable, Category = "Board")
    void EnableMinigameInput();

    UFUNCTION(BlueprintCallable, Category = "Board")
    void DisableMinigameInput();

    UFUNCTION(BlueprintPure, Category = "Board")
    bool IsInputEnabled() const { return bInputEnabled; }

    // For pawn to find
    UFUNCTION(BlueprintCallable, Category = "Board")
    bool IsGameOver() const { return bGameOver; }

    UFUNCTION(BlueprintPure, Category = "Board")
    bool IsPuzzleSolved() const { return Logic && Logic->IsPuzzleSolved(); }

    // Root component for the actor
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    USceneComponent* SceneRoot;

    // Trigger component for automatic input enabling/disabling
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    UBoardTrigger* TriggerComponent;

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

    // Optional: Auto-enable input when game starts (no trigger needed)
    UPROPERTY(EditAnywhere, Category="Board")
    bool bAutoEnableInput = false;

    // Use trigger box for proximity-based activation
    UPROPERTY(EditAnywhere, Category="Board")
    bool bUseTriggerBox = true;

protected:
    // Input handlers
    void MoveLeft();
    void MoveRight();

    // Blueprint events for UI feedback
    UFUNCTION(BlueprintImplementableEvent, Category = "Board")
    void OnMinigameActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Board")
    void OnMinigameDeactivated();

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
    bool bInputEnabled = false;

    // For simplest lock detection: keep previous grid snapshot
    TArray<ETile> PrevGrid;
};
