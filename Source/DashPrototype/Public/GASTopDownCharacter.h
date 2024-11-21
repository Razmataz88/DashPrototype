// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DashAttributeSet.h"
#include "GA_Dash_Native.h"
#include "GameFramework/Character.h"
#include "GASTopDownCharacter.generated.h"

UCLASS()
class DASHPROTOTYPE_API AGASTopDownCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASTopDownCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ActivateDashAbility();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UDashAttributeSet* DashAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> DashAbilityClass;
};
