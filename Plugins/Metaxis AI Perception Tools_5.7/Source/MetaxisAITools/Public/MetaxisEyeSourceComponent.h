#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaxisEyeSourceComponent.generated.h"

class APawn;
class AAIController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METAXISAITOOLS_API UMetaxisEyeSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMetaxisEyeSourceComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metaxis AI Perception Tools|Eye Source Component")
	FName EyeSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metaxis AI Perception Tools|Eye Source Component")
	bool bEnableOverrideEyesViewpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metaxis AI Perception Tools|Eye Source Component")
	bool bFlattenSightForwardAxis;

	bool GetCustomEyesViewPoint(const APawn* TargetPawn, FVector& OutLocation, FRotator& OutRotation) const;

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Eye Source Component")
	void SetFlattenSightForwardAxis(AAIController* OwningController, bool bNewEnabled);
};