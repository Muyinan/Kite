// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "KitePlayerState.generated.h"

class UKiteAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class KITE_API AKitePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AKitePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerState")
	UKiteAbilitySystemComponent* GetKiteAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Kite|PlayerState")
	TObjectPtr<UKiteAbilitySystemComponent> AbilitySystemComponent;
};
