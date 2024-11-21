// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash_Native.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "DashAttributeSet.h"
#include "VectorTypes.h"
#include "Kismet/KismetSystemLibrary.h"

bool UGA_Dash_Native::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!CanDash(ActorInfo->AbilitySystemComponent.Get()))
	{
		return false;
	}
	
	return true;
}

void UGA_Dash_Native::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (GEDashDecrement)
	{
		// Apply the Gameplay Effect to the owner
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GEDashDecrement, 1.0f, ASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

	if (!DashMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Dash", DashMontage);
	if (MontageTask)
	{
		MontageTask->Activate();
	}

	FVector Location = GetDashDestination();
	UAbilityTask_MoveToLocation* MoveToTask = UAbilityTask_MoveToLocation::MoveToLocation(
		this,
		FName("MoveToTarget"),
		Location,
		Duration,	// Duration
		nullptr,	// OptionalInterpolationCurve
		nullptr		// OptionalVectorInterpolationCurve
	);
	if (MoveToTask)
	{
		MoveToTask->ReadyForActivation();
		MoveToTask->OnTargetLocationReached.AddDynamic(this, &UGA_Dash_Native::OnMoveCompleted);

	}
	else
	{
		EndAbility(Handle, ActorInfo,ActivationInfo, false, false);
	}

	if (GEDashCooldown)
	{
		// Apply the Gameplay Effect to the owner
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GEDashCooldown, 1.0f, ASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void UGA_Dash_Native::OnMoveCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool UGA_Dash_Native::CanDash(const UAbilitySystemComponent* asc) const
{
	if (!asc)
	{
		return false;
	}
	
	const UDashAttributeSet* attributeSet = asc->GetSet<UDashAttributeSet>();
	if (!attributeSet)
	{
		return false;
	}

	return attributeSet->GetAvailableDashCount() > 0.0f;
}

FVector UGA_Dash_Native::GetDashDestination() const
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (!OwningActor)
	{
		// Error, this shouldn't be hit
		return FVector::ZeroVector;
	}

	FVector StartLocation = OwningActor->GetActorLocation();
	FVector EndLocation = StartLocation + (OwningActor->GetActorForwardVector() * DashDuration);

	return FindFurthestDestinationAlongPath(StartLocation, EndLocation);
}

bool UGA_Dash_Native::IsValidDashLocation(const FVector& Location, const TArray<FHitResult>& HitResults) const
{
	FVector FloorCheck = Location + FVector(0,0,-500);
	FCollisionQueryParams CollisionParams; // define the collision
	FHitResult Hit;
	
	bool hasFloor = GetWorld()->LineTraceSingleByChannel(Hit, Location, FloorCheck, ECC_Visibility, CollisionParams);
	if (!hasFloor)
	{
		return false;
	}
	
	if (HitResults.IsEmpty())
	{
		return true;
	}

	for (FHitResult HitResult : HitResults)
	{
		UPrimitiveComponent* HitComponent = HitResult.Component.Get();
		if (HitComponent)
		{
			if (HitComponent->GetCollisionResponseToChannel(ECC_Pawn) == ECollisionResponse::ECR_Block)
			{
				return false;
			}
			
		}
	}

	return true;
}

FVector UGA_Dash_Native::FindFurthestDestinationAlongPath(const FVector& StartLocation, const FVector& EndLocation) const
{
	FVector Direction = EndLocation - StartLocation;
	FVector UnitVector = Direction.GetSafeNormal();

	//Get Radius and height
	float CapsuleDiameter = 84.0f;

	int CapsuleCount = Direction.Size() / CapsuleDiameter;

	for (int i = 0; i < CapsuleCount; i++)
	{
		int index = CapsuleCount - i;
		FVector CurrentCapsuleLocation = StartLocation + (UnitVector * index * CapsuleDiameter);
		TArray<FHitResult> HitResults;
		TArray<AActor*> ActorsToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> Query;
		Query.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		Query.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		Query.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
		Query.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		Query.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

		UKismetSystemLibrary::CapsuleTraceMultiForObjects(
			GetWorld(),
			CurrentCapsuleLocation,
			CurrentCapsuleLocation, 
			CapsuleDiameter / 2.0f,
			96,
			Query,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults, true
			);
		
		if (IsValidDashLocation(CurrentCapsuleLocation, HitResults))
		{
			return CurrentCapsuleLocation;
		}
	}
	return StartLocation;
}
