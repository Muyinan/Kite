// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "KitePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API AKitePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKitePlayerState();

	UPROPERTY()
	class UKiteAbilitySystemComponent* AbilitySystemComponent;

public:
	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End IAbilitySystemInterface
};
