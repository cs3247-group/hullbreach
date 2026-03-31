#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MetaxisBlueprintFunctionLibrary.generated.h"

class UAIPerceptionComponent;
class AActor;

/**
 * Blueprint Function Library for Metaxis AI Perception Tools.
 */
UCLASS()
class UMetaxisBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Forget Functions
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools", meta=(DisplayName = "AI Forget Actor"))
	static void AIForgetActor(UAIPerceptionComponent* AIPerceptionComponent, AActor* ActorToForget);
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools", meta=(DisplayName = "AI Forget Actors"))
	static void AIForgetActors(UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& ActorsToForget);

	// Set Sight Config Functions
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Vision Cone Half Angle"))
	static void SetVisionConeHalfAngle(UAIPerceptionComponent* AIPerceptionComponent, float NewHalfAngle);
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Vision Cone Sight Radius"))
	static void SetVisionConeSightRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewRadius);
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Vision Cone Lose Sight Radius"))
	static void SetVisionConeLoseSightRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewLoseSightRadius);
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Point Of View Backward Offset"))
	static void SetPointOfViewBackwardOffset(UAIPerceptionComponent* AIPerceptionComponent, float NewOffset);
	
	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Near Clipping Radius"))
	static void SetNearClippingRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewRadius);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Set Sight Sense Max Age"))
	static void SetSightSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge);

	// Get Sight Config Functions

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Vision Cone Half Angle"))
	static float GetVisionConeHalfAngle(UAIPerceptionComponent* AIPerceptionComponent);
	
	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Vision Cone Sight Radius"))
	static float GetVisionConeSightRadius(UAIPerceptionComponent* AIPerceptionComponent);
	
	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Vision Cone Lose Sight Radius"))
	static float GetVisionConeLoseSightRadius(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Point Of View Backward Offset"))
	static float GetPointOfViewBackwardOffset(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Near Clipping Radius"))
	static float GetNearClippingRadius(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Sight Config", meta=(DisplayName = "Get Sight Sense Max Age"))
	static float GetSightSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent);

	// Hearing Config Functions
	
	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Hearing Config", meta=(DisplayName = "Get Hearing Range"))
	static float GetHearingRange(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Hearing Config", meta=(DisplayName = "Set Hearing Range"))
	static void SetHearingRange(UAIPerceptionComponent* AIPerceptionComponent, float NewHearingRange);

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Hearing Config", meta=(DisplayName = "Get Hearing Sense Max Age"))
	static float GetHearingSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Hearing Config", meta=(DisplayName = "Set Hearing Sense Max Age"))
	static void SetHearingSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge);
	
	// Damage Config Functions

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Damage Config", meta=(DisplayName = "Get Damage Sense Max Age"))
	static float GetDamageSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Damage Config", meta=(DisplayName = "Set Damage Sense Max Age"))
	static void SetDamageSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge);

	// Touch Config Functions

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|Touch Config", meta=(DisplayName = "Get Touch Sense Max Age"))
	static float GetTouchSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|Touch Config", meta=(DisplayName = "Set Touch Sense Max Age"))
	static void SetTouchSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge);

	// General Perception Functions2

	UFUNCTION(BlueprintPure, Category = "Metaxis AI Perception Tools|General", meta=(DisplayName = "Get Dominant Sense"))
	static TSubclassOf<UAISense> GetDominantSense(UAIPerceptionComponent* AIPerceptionComponent);

	UFUNCTION(BlueprintCallable, Category = "Metaxis AI Perception Tools|General", meta=(DisplayName = "Set Dominant Sense"))
	static void SetDominantSense(UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<UAISense> NewDominantSense);
};