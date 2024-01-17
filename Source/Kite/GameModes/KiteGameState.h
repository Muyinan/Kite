// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "KiteGameState.generated.h"

class UKiteAbilitySystemComponent;
class UKiteExperienceManagerComponent;
/**
 * 
 */
UCLASS(Config = Game)
class KITE_API AKiteGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:

	AKiteGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Gets the ability system component used for game wide things
    UFUNCTION(BlueprintCallable, Category = "Kite|GameState")
    UKiteAbilitySystemComponent* GetKiteAbilitySystemComponent() const { return AbilitySystemComponent; }
	//
	// TObjectPtr<UKiteExperienceManagerComponent> GetExperienceManagerComponent() const { return ExperienceManagerComponent; }
private:
	// Handles loading and managing the current gameplay experience
	UPROPERTY()
	TObjectPtr<UKiteExperienceManagerComponent> ExperienceManagerComponent;

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "Kite|GameState")
	TObjectPtr<UKiteAbilitySystemComponent> AbilitySystemComponent;
};
