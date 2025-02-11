// Fill out your copyright notice in the Description page of Project Settings.


#include "GASTopDownCharacter.h"
#include "AbilitySystemComponent.h"
#include "GA_Dash_Native.h"
#include "DashAttributeSet.h"

// Sets default values
AGASTopDownCharacter::AGASTopDownCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	if (DashAbilityClass)
	{
		FGameplayAbilitySpec AbilitySpec(DashAbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

// Called when the game starts or when spawned
void AGASTopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!DashAttributeSet)
	{
		DashAttributeSet = NewObject<UDashAttributeSet>(this);
		AbilitySystemComponent->AddSet<UDashAttributeSet>();
	}
	if (DashAbilityClass)
	{
		FGameplayAbilitySpec AbilitySpec(DashAbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

// Called every frame
void AGASTopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASTopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("DashAbility"), IE_Pressed, this, &AGASTopDownCharacter::ActivateDashAbility);

}

void AGASTopDownCharacter::ActivateDashAbility()
{
	if (AbilitySystemComponent && DashAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(DashAbilityClass);
	}
}

