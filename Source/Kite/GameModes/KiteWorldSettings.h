// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "KiteWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API AKiteWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	AKiteWorldSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Returns the default experience to use when a server opens this map if it is not overridden by the user-facing experience
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	// The default experience to use when a server opens this map if it is not overridden by the user-facing experience
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<class UKiteExperienceDefinition> DefaultGameplayExperience;

};
