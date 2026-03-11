// CircuitPuzzleManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircuitPuzzleManager.generated.h"

// Port connections as bitmask
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPortMask : uint8
{
    None = 0,
    N = 1 << 0,  // 1
    E = 1 << 1,  // 2
    S = 1 << 2,  // 4
    W = 1 << 3   // 8
};
ENUM_CLASS_FLAGS(EPortMask)

// Cube types
UENUM(BlueprintType)
enum class ECubeType : uint8
{
    Straight,  // Connects opposite sides (N-S or E-W)
    Corner     // Connects perpendicular sides (90° turn)
};

// Grid cell structure
USTRUCT(BlueprintType)
struct FCircuitCell
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bOccupied = false;

    UPROPERTY(BlueprintReadOnly)
    ECubeType Type = ECubeType::Straight;

    UPROPERTY(BlueprintReadOnly)
    uint8 Rotation = 0;

    UPROPERTY(BlueprintReadOnly)
    EPortMask Ports = EPortMask::None;

    UPROPERTY(BlueprintReadOnly)
    bool bPowered = false;

    void Reset()
    {
        bOccupied = false;
        bPowered = false;
        Rotation = 0;
        Ports = EPortMask::None;
    }
};

// Active falling cube
USTRUCT(BlueprintType)
struct FActiveCube
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bActive = false;

    UPROPERTY(BlueprintReadOnly)
    ECubeType Type = ECubeType::Straight;

    UPROPERTY(BlueprintReadOnly)
    uint8 Rotation = 0;

    UPROPERTY(BlueprintReadOnly)
    EPortMask Ports = EPortMask::None;

    UPROPERTY(BlueprintReadOnly)
    FIntPoint Coord = FIntPoint::ZeroValue;
};

UCLASS(BlueprintType, Blueprintable)
class GAMEDEVGAME_API ACircuitPuzzleManager : public AActor
{
    GENERATED_BODY()

public:
    ACircuitPuzzleManager();

    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridWidth = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridHeight = 20;

    // Power and bypass node positions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FIntPoint PowerNode = FIntPoint(0, 19);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FIntPoint BypassNode = FIntPoint(9, 0);

    // Fall speed (seconds per step)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float FallSpeed = 0.5f;

    // Game state
    UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
    bool bGameWon = false;

    UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
    bool bGameActive = false;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Game control
    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void ResetGame();

    // Player input
    UFUNCTION(BlueprintCallable, Category = "Input")
    void MoveLeft();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void MoveRight();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void RotateCube();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void DropCube();

    // Grid access
    UFUNCTION(BlueprintPure, Category = "Grid")
    FCircuitCell GetCell(int32 X, int32 Y) const;

    UFUNCTION(BlueprintPure, Category = "Grid")
    FActiveCube GetActiveCube() const { return ActiveCube; }

    // Events (implement in Blueprint)
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnCubeLocked(int32 X, int32 Y);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnPowerChanged();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnGameWon();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnCubeSpawned();

private:
    // Grid storage
    UPROPERTY()
    TArray<FCircuitCell> Grid;

    // Active cube
    UPROPERTY()
    FActiveCube ActiveCube;

    // Timer
    FTimerHandle FallTimerHandle;
    float FallAccumulator = 0.0f;

    // Helper functions
    int32 GetIndex(int32 X, int32 Y) const;
    bool IsValidCoord(int32 X, int32 Y) const;
    bool CanPlace(int32 X, int32 Y) const;
    
    EPortMask ComputePorts(ECubeType Type, uint8 Rotation) const;
    void SpawnNextCube();
    void StepFall();
    void LockCube();
    void ComputePower();
    void Move(int32 DX);

    // Helper for port checking
    EPortMask GetOppositePort(EPortMask Port) const;
    FIntPoint GetNeighborCoord(FIntPoint Coord, EPortMask Direction) const;
};
