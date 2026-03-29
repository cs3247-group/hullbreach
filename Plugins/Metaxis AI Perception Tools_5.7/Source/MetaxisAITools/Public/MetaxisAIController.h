#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MetaxisAIController.generated.h"

class UMetaxisEyeSourceComponent;

/**
 * A custom AI Controller that expands perception functionality.
 */
UCLASS()
class METAXISAITOOLS_API AMetaxisAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMetaxisAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

protected:

    UPROPERTY(Category = "AI", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UMetaxisEyeSourceComponent> EyeSourceComponent;
};