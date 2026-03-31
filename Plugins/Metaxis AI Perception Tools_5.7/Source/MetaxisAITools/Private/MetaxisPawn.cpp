// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaxisPawn.h"
#include "MetaxisEyeSourceComponent.h"


AMetaxisPawn::AMetaxisPawn()
{

	PrimaryActorTick.bCanEverTick = true;

	EyeSourceComponent = CreateDefaultSubobject<UMetaxisEyeSourceComponent>(TEXT("EyeSourceComponent"));

}

void AMetaxisPawn::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{

	if (IsValid(EyeSourceComponent) && EyeSourceComponent->GetCustomEyesViewPoint(this, OutLocation, OutRotation))
	{
		return;
	}
    
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

// Called when the game starts or when spawned
void AMetaxisPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaxisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMetaxisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

