// Fill out your copyright notice in the Description page of Project Settings.

#include "DashAttributeSet.h"

UDashAttributeSet::UDashAttributeSet()
	: AvailableDashCount(3.f)
	, MaxDashCount(3.f)
	, DashDuration(0.33f)
	, MaxDashDistance(1000.f)
{
	
}

void UDashAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAvailableDashCountAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDashCount());
	}
	else if (Attribute == GetMaxDashCountAttribute())
	{
		NewValue = FMath::Max(NewValue, 3);
	}
}