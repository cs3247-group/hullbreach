#include "MetaxisEyeSourceComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMetaxisEyeSourceComponent::UMetaxisEyeSourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bEnableOverrideEyesViewpoint = true;
	EyeSocketName = TEXT("Eyes");
	bFlattenSightForwardAxis = false;
}

bool UMetaxisEyeSourceComponent::GetCustomEyesViewPoint(const APawn* TargetPawn, FVector& OutLocation, FRotator& OutRotation) const
{

	if (!TargetPawn || !bEnableOverrideEyesViewpoint)
	{
		return false;
	}

	USkeletalMeshComponent* TargetSkeletalMesh = nullptr;
	const ACharacter* OwnerCharacter = Cast<ACharacter>(TargetPawn);
	if (OwnerCharacter)
	{
		TargetSkeletalMesh = OwnerCharacter->GetMesh();
	}
	else
	{
		TargetSkeletalMesh = TargetPawn->FindComponentByClass<USkeletalMeshComponent>();
	}

	if (TargetSkeletalMesh && TargetSkeletalMesh->DoesSocketExist(EyeSocketName))
	{
		FTransform SocketTransform = TargetSkeletalMesh->GetSocketTransform(EyeSocketName);
		OutLocation = SocketTransform.GetLocation();

		if (bFlattenSightForwardAxis)
		{
			FVector SocketForwardVector = SocketTransform.GetRotation().GetForwardVector();
			FVector FlatForwardVector = SocketForwardVector;
			FlatForwardVector.Z = 0.0f;

			if (!FlatForwardVector.IsNearlyZero())
			{
				FlatForwardVector.Normalize();
			}
			else
			{
				FlatForwardVector = TargetPawn->GetActorForwardVector();
				FlatForwardVector.Z = 0.0f;
				if (!FlatForwardVector.IsNearlyZero())
				{
					FlatForwardVector.Normalize();
				}
				else
				{
					FlatForwardVector = FVector::ForwardVector;
				}
			}
			OutRotation = UKismetMathLibrary::MakeRotFromX(FlatForwardVector);
		}
		else
		{
			OutRotation = SocketTransform.GetRotation().Rotator();
		}
		return true; 
	}
	return false; 
}

void UMetaxisEyeSourceComponent::SetFlattenSightForwardAxis(AAIController* OwningController, bool bNewEnabled)
{
	bFlattenSightForwardAxis = bNewEnabled;
	UE_LOG(LogTemp, Log, TEXT("UMetaxisEyeSourceComponent '%s': Set Compensate Sight Forward Axis to %s."),
		   *GetName(), bNewEnabled ? TEXT("Enabled") : TEXT("Disabled"));

	if (OwningController)
	{
		UAIPerceptionComponent* PerceptionComp = OwningController->GetPerceptionComponent();
		if (PerceptionComp)
		{
			PerceptionComp->RequestStimuliListenerUpdate();
		}
	}
}