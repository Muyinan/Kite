// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "KiteExperienceManager.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UKiteExperienceManager : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static void NotifyOfPluginActivation(const FString PluginURL);

private:
	// The map of requests to active count for a given game feature plugin
	// (to allow first in, last out activation management during PIE)
	TMap<FString, int32> GameFeaturePluginRequestCountMap;
};
