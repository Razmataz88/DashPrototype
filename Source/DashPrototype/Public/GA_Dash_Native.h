// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Dash_Native.generated.h"

/**
 * 
 */
UCLASS()
class DASHPROTOTYPE_API UGA_Dash_Native : public UGameplayAbility
{
	GENERATED_BODY()
public:

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMoveCompleted();
	
	UFUNCTION()
	bool CanDash(const UAbilitySystemComponent* asc) const;
	
	UFUNCTION()
	FVector GetDashDestination() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<UGameplayEffect> GEDashDecrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<UGameplayEffect> GEDashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration;
	
private:
	UFUNCTION()
	bool IsValidDashLocation(const FVector& Location, const TArray<FHitResult>& HitResults) const;

	UFUNCTION()
	FVector FindFurthestDestinationAlongPath(const FVector& StartLocation, const FVector& EndLocation) const;
};
