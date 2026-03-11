// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeBlockActor.h"
#include "Components/StaticMeshComponent.h"

ACubeBlockActor::ACubeBlockActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACubeBlockActor::SetType(ETile InType)
{
    Type = InType;

    if (Type == ETile::Straight && MatStraight)
    {
        Mesh->SetMaterial(0, MatStraight);
    }
    else if (Type == ETile::Cross && MatCross)
    {
        Mesh->SetMaterial(0, MatCross);
    }
}
