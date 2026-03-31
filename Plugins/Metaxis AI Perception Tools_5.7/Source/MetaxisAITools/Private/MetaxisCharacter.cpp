// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaxisCharacter.h"
#include "MetaxisEyeSourceComponent.h"

AMetaxisCharacter::AMetaxisCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	EyeSourceComponent = CreateDefaultSubobject<UMetaxisEyeSourceComponent>(TEXT("EyeSourceComponent"));

}

void AMetaxisCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{

	if (IsValid(EyeSourceComponent) && EyeSourceComponent->GetCustomEyesViewPoint(this, OutLocation, OutRotation))
	{
		return;
	}
    
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

// Called when the game starts or when spawned
void AMetaxisCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaxisCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMetaxisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

