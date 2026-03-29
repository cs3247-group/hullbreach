#include "MetaxisAIController.h"
#include "MetaxisEyeSourceComponent.h" // Include component
#include "GameFramework/Pawn.h"

// ADD COMPONENT IN CONSTRUCTOR
AMetaxisAIController::AMetaxisAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    EyeSourceComponent = CreateDefaultSubobject<UMetaxisEyeSourceComponent>(TEXT("EyeSourceComponent"));
}

// UPDATE LOGIC
void AMetaxisAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        Super::GetActorEyesViewPoint(OutLocation, OutRotation);
        return;
    }

    if (EyeSourceComponent && EyeSourceComponent->GetCustomEyesViewPoint(ControlledPawn, OutLocation, OutRotation))
    {
        return; // Controller's component handled the override.
    }

    Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}