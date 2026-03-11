// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputPawn.generated.h"

class ABoardActor;

UCLASS()
class GAMEDEVGAME_API AInputPawn : public APawn
{
    GENERATED_BODY()

public:
    AInputPawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;

    void MoveLeft();
    void MoveRight();

private:
    ABoardActor* FindBoard() const;

private:
    UPROPERTY()
    ABoardActor* Board = nullptr;
};