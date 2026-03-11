// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardLogic.h"
#include "CubeBlockActor.generated.h"

UCLASS()
class GAMEDEVGAME_API ACubeBlockActor : public AActor
{
    GENERATED_BODY()

public:
    ACubeBlockActor();

    void SetType(ETile InType);

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, Category="Visual")
    UMaterialInterface* MatStraight;

    UPROPERTY(EditDefaultsOnly, Category="Visual")
    UMaterialInterface* MatCross;

private:
    ETile Type = ETile::Empty;
};
