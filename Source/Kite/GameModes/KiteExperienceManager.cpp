// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/KiteExperienceManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(KiteExperienceManager)

void UKiteExperienceManager::NotifyOfPluginActivation(const FString PluginURL)
{
	if (GIsEditor)
	{
		UKiteExperienceManager* ExperienceManagerSubsystem = GEngine->GetEngineSubsystem<UKiteExperienceManager>();
		check(ExperienceManagerSubsystem);

		// Track the number of requesters who activate this plugin. Multiple load/activation requests are always allowed because concurrent requests are handled.
		int32& Count = ExperienceManagerSubsystem->GameFeaturePluginRequestCountMap.FindOrAdd(PluginURL);
		++Count;
	}
}
