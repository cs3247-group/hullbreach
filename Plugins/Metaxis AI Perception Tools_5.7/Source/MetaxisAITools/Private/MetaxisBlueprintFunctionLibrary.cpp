#include "MetaxisBlueprintFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"


// Forget Functions

void UMetaxisBlueprintFunctionLibrary::AIForgetActor(UAIPerceptionComponent* AIPerceptionComponent, AActor* ActorToForget)
{
	if (!IsValid(AIPerceptionComponent) || !IsValid(ActorToForget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Forget Actor: Invalid AIPerceptionComponent or ActorToForget provided."));
		return;
	}

	FString ActorName = ActorToForget->GetName();

	AIPerceptionComponent->ForgetActor(ActorToForget);
    
	AIPerceptionComponent->RequestStimuliListenerUpdate();

	UE_LOG(LogTemp, Log, TEXT("AI Forget Actor: Successfully told component to forget actor %s."), *ActorName);
}

void UMetaxisBlueprintFunctionLibrary::AIForgetActors(UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& ActorsToForget)
{
    if (!IsValid(AIPerceptionComponent))
    {
       UE_LOG(LogTemp, Warning, TEXT("AI Forget Actors: Invalid AIPerceptionComponent provided."));
       return;
    }

    // Create a local copy of the array. This protects the loop from external modifications caused by ForgetActor triggering delegates.
    TArray<AActor*> LocalActorsToForget = ActorsToForget;
    
    bool bAnyForgotten = false;

    // Iterate over the COPY (LocalActorsToForget), not the input reference
    for (AActor* CurrentActor : LocalActorsToForget)
    {
       if (IsValid(CurrentActor))
       {
          // Triggers delegates that might modify the original array
          AIPerceptionComponent->ForgetActor(CurrentActor); 
          bAnyForgotten = true;
       }
    }
    
    if (bAnyForgotten) 
    {
       UE_LOG(LogTemp, Log, TEXT("AI Forget Actors: Processed forget requests for component %s."), *AIPerceptionComponent->GetName());
       AIPerceptionComponent->RequestStimuliListenerUpdate();
    }
}


// --- Set Sight Config Functions ---

void UMetaxisBlueprintFunctionLibrary::SetVisionConeHalfAngle(UAIPerceptionComponent* AIPerceptionComponent, float NewHalfAngle)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeHalfAngle: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		SightConfig->PeripheralVisionAngleDegrees = FMath::Clamp(NewHalfAngle, 0.0f, 180.0f);
		
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetVisionConeHalfAngle: Successfully set sight sense half angle to %f degrees for component %s."),
			NewHalfAngle, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeHalfAngle: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}

void UMetaxisBlueprintFunctionLibrary::SetVisionConeSightRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewRadius)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeSightRadius: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		SightConfig->SightRadius = FMath::Max(0.0f, NewRadius);
		
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();

		UE_LOG(LogTemp, Log, TEXT("SetVisionConeSightRadius: Successfully set sight sense radius to %f for component %s."),
			NewRadius, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeSightRadius: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}

void UMetaxisBlueprintFunctionLibrary::SetVisionConeLoseSightRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewLoseSightRadius)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeLoseSightRadius: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
    
	if (SightConfig)
	{
		SightConfig->LoseSightRadius = FMath::Max(0.0f, NewLoseSightRadius);
        
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();

		UE_LOG(LogTemp, Log, TEXT("SetVisionConeLoseSightRadius: Successfully set sight sense lose sight radius to %f for component %s."),
			NewLoseSightRadius, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetVisionConeLoseSightRadius: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}

void UMetaxisBlueprintFunctionLibrary::SetPointOfViewBackwardOffset(UAIPerceptionComponent* AIPerceptionComponent, float NewOffset)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPointOfViewBackwardOffset: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		SightConfig->PointOfViewBackwardOffset = FMath::Max(0.0f, NewOffset);
		
		AIPerceptionComponent->ConfigureSense(*SightConfig); 
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetPointOfViewBackwardOffset: Successfully set sight sense POV backward offset to %f for component %s."),
			NewOffset, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPointOfViewBackwardOffset: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}

void UMetaxisBlueprintFunctionLibrary::SetNearClippingRadius(UAIPerceptionComponent* AIPerceptionComponent, float NewRadius)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetNearClippingRadius: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		SightConfig->NearClippingRadius = FMath::Max(0.0f, NewRadius);
		
		AIPerceptionComponent->ConfigureSense(*SightConfig); 
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetNearClippingRadius: Successfully set sight sense near clipping radius to %f for component %s."),
			NewRadius, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetNearClippingRadius: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}

void UMetaxisBlueprintFunctionLibrary::SetSightSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSightSenseMaxAge: Invalid AIPerceptionComponent provided."));
		return;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		SightConfig->SetMaxAge(FMath::Max(0.0f, NewMaxAge));
		
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetSightSenseMaxAge: Successfully set sight sense max age to %f for component %s."),
			NewMaxAge, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSightSenseMaxAge: No UAISenseConfig_Sight found on component %s. Ensure it has a Sight Sense configured."), *AIPerceptionComponent->GetName());
	}
}


// --- Get Sight Config Functions ---


float UMetaxisBlueprintFunctionLibrary::GetVisionConeHalfAngle(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVisionConeHalfAngle: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		return SightConfig->PeripheralVisionAngleDegrees;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetVisionConeHalfAngle: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

float UMetaxisBlueprintFunctionLibrary::GetVisionConeSightRadius(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVisionConeSightRadius: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		return SightConfig->SightRadius;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetVisionConeSightRadius: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

float UMetaxisBlueprintFunctionLibrary::GetVisionConeLoseSightRadius(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVisionConeLoseSightRadius: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
    
	if (SightConfig)
	{
		return SightConfig->LoseSightRadius;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetVisionConeLoseSightRadius: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

float UMetaxisBlueprintFunctionLibrary::GetPointOfViewBackwardOffset(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPointOfViewBackwardOffset: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		return SightConfig->PointOfViewBackwardOffset;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetPointOfViewBackwardOffset: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

float UMetaxisBlueprintFunctionLibrary::GetNearClippingRadius(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetNearClippingRadius: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		return SightConfig->NearClippingRadius;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetNearClippingRadius: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

float UMetaxisBlueprintFunctionLibrary::GetSightSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSightSenseMaxAge: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	
	if (SightConfig)
	{
		return SightConfig->GetMaxAge();
	}

	UE_LOG(LogTemp, Warning, TEXT("GetSightSenseMaxAge: No UAISenseConfig_Sight found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}


// --- Hearing Config Functions ---

float UMetaxisBlueprintFunctionLibrary::GetHearingRange(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHearingRange: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}
    
	UAISenseConfig_Hearing* HearingConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Hearing>();
        
	if (HearingConfig)
	{
		return HearingConfig->HearingRange;
	}
    
	UE_LOG(LogTemp, Warning, TEXT("GetHearingRange: No UAISenseConfig_Hearing found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}


void UMetaxisBlueprintFunctionLibrary::SetHearingRange(UAIPerceptionComponent* AIPerceptionComponent, float NewHearingRange)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHearingRange: Invalid AIPerceptionComponent provided. Cannot set hearing range."));
		return;
	}

	UAISenseConfig_Hearing* HearingConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Hearing>();
	
	if (HearingConfig)
	{
		HearingConfig->HearingRange = FMath::Max(0.0f, NewHearingRange); 
		
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetHearingRange: Successfully set hearing sense range to %f for component %s."),
			NewHearingRange, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHearingRange: No UAISenseConfig_Hearing found on component %s. Ensure it has a Hearing Sense configured."), *AIPerceptionComponent->GetName());
	}
}

float UMetaxisBlueprintFunctionLibrary::GetHearingSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHearingSenseMaxAge: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}
    
	UAISenseConfig_Hearing* HearingConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Hearing>();
        
	if (HearingConfig)
	{
		return HearingConfig->GetMaxAge();
	}
    
	UE_LOG(LogTemp, Warning, TEXT("GetHearingSenseMaxAge: No UAISenseConfig_Hearing found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

void UMetaxisBlueprintFunctionLibrary::SetHearingSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHearingSenseMaxAge: Invalid AIPerceptionComponent provided. Cannot set max age."));
		return;
	}

	UAISenseConfig_Hearing* HearingConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Hearing>();
	
	if (HearingConfig)
	{
		HearingConfig->SetMaxAge(FMath::Max(0.0f, NewMaxAge)); 
		
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetHearingSenseMaxAge: Successfully set hearing sense max age to %f for component %s."),
			NewMaxAge, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHearingSenseMaxAge: No UAISenseConfig_Hearing found on component %s. Ensure it has a Hearing Sense configured."), *AIPerceptionComponent->GetName());
	}
}


// --- Damage Config Functions ---

float UMetaxisBlueprintFunctionLibrary::GetDamageSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetDamageSenseMaxAge: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}
    
	UAISenseConfig_Damage* DamageConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Damage>();
        
	if (DamageConfig)
	{
		return DamageConfig->GetMaxAge();
	}
    
	UE_LOG(LogTemp, Warning, TEXT("GetDamageSenseMaxAge: No UAISenseConfig_Damage found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

void UMetaxisBlueprintFunctionLibrary::SetDamageSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetDamageSenseMaxAge: Invalid AIPerceptionComponent provided. Cannot set max age."));
		return;
	}

	UAISenseConfig_Damage* DamageConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Damage>();
	
	if (DamageConfig)
	{
		DamageConfig->SetMaxAge(FMath::Max(0.0f, NewMaxAge)); 
		
		AIPerceptionComponent->ConfigureSense(*DamageConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetDamageSenseMaxAge: Successfully set damage sense max age to %f for component %s."),
			NewMaxAge, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetDamageSenseMaxAge: No UAISenseConfig_Damage found on component %s. Ensure it has a Damage Sense configured."), *AIPerceptionComponent->GetName());
	}
}


// --- Touch Config Functions ---

float UMetaxisBlueprintFunctionLibrary::GetTouchSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetTouchSenseMaxAge: Invalid AIPerceptionComponent provided. Returning 0.0f."));
		return 0.0f;
	}
    
	UAISenseConfig_Touch* TouchConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Touch>();
        
	if (TouchConfig)
	{
		return TouchConfig->GetMaxAge();
	}
    
	UE_LOG(LogTemp, Warning, TEXT("GetTouchSenseMaxAge: No UAISenseConfig_Touch found on component %s. Returning 0.0f."), *AIPerceptionComponent->GetName());
	return 0.0f;
}

void UMetaxisBlueprintFunctionLibrary::SetTouchSenseMaxAge(UAIPerceptionComponent* AIPerceptionComponent, float NewMaxAge)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTouchSenseMaxAge: Invalid AIPerceptionComponent provided. Cannot set max age."));
		return;
	}

	UAISenseConfig_Touch* TouchConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Touch>();
	
	if (TouchConfig)
	{
		TouchConfig->SetMaxAge(FMath::Max(0.0f, NewMaxAge)); 
		
		AIPerceptionComponent->ConfigureSense(*TouchConfig);
		AIPerceptionComponent->RequestStimuliListenerUpdate();
		
		UE_LOG(LogTemp, Log, TEXT("SetTouchSenseMaxAge: Successfully set touch sense max age to %f for component %s."),
			NewMaxAge, *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTouchSenseMaxAge: No UAISenseConfig_Touch found on component %s. Ensure it has a Touch Sense configured."), *AIPerceptionComponent->GetName());
	}
}

// --- General Perception Functions ---

TSubclassOf<UAISense> UMetaxisBlueprintFunctionLibrary::GetDominantSense(UAIPerceptionComponent* AIPerceptionComponent)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetDominantSense: Invalid AIPerceptionComponent provided. Returning nullptr."));
		return nullptr;
	}

	return AIPerceptionComponent->GetDominantSense();
}

void UMetaxisBlueprintFunctionLibrary::SetDominantSense(UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<UAISense> NewDominantSense)
{
	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetDominantSense: Invalid AIPerceptionComponent provided. Cannot set dominant sense."));
		return;
	}
	
	AIPerceptionComponent->SetDominantSense(NewDominantSense);
	
	if (NewDominantSense)
	{
		UE_LOG(LogTemp, Log, TEXT("SetDominantSense: Successfully set dominant sense to %s for component %s."),
			*NewDominantSense->GetName(), *AIPerceptionComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SetDominantSense: Successfully cleared dominant sense for component %s."),
			*AIPerceptionComponent->GetName());
	}
}