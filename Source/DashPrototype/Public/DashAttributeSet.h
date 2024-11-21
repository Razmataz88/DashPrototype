// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DashAttributeSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DASHPROTOTYPE_API UDashAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	UDashAttributeSet();

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDashAttributeSet, AvailableDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AvailableDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AvailableDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AvailableDashCount)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDashAttributeSet, MaxDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxDashCount) 
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxDashCount)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDashAttributeSet, DashDuration) 
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DashDuration) 
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DashDuration) 
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DashDuration)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDashAttributeSet, MaxDashDistance) 
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxDashDistance) 
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxDashDistance) 
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxDashDistance)
	

	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay|Dash", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AvailableDashCount;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay|Dash", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxDashCount;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay|Dash", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData DashDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay|Dash", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxDashDistance;
};
