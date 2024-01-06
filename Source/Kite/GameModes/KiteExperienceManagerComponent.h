// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "KiteExperienceManagerComponent.generated.h"

namespace UE::GameFeatures
{
	struct FResult;
}

class UKiteExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnKiteExperienceLoaded, const UKiteExperienceDefinition* /*Experience*/);

enum class EKiteExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

/**
 * 
 */
UCLASS()
class KITE_API UKiteExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UKiteExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Returns true if the experience is fully loaded
	bool IsExperienceLoaded() const;

	// This returns the current experience if it is fully loaded, asserting otherwise
	// (i.e., if you called it too soon)
	const UKiteExperienceDefinition* GetCurrentExperienceChecked() const;

	// Tries to set the current experience, either a UI or gameplay one
	void SetCurrentExperience(FPrimaryAssetId ExperienceId);
	
	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnExperienceLoaded(FOnKiteExperienceLoaded::FDelegate&& Delegate);
	
private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	
private:
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UKiteExperienceDefinition> CurrentExperience;
	
	EKiteExperienceLoadState LoadState = EKiteExperienceLoadState::Unloaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	/**
 * Delegate called when the experience has finished loading just before others
 * (e.g., subsystems that set up for regular gameplay)
 */
	FOnKiteExperienceLoaded OnExperienceLoaded_HighPriority;

	/** Delegate called when the experience has finished loading */
	FOnKiteExperienceLoaded OnExperienceLoaded;

	/** Delegate called when the experience has finished loading */
	FOnKiteExperienceLoaded OnExperienceLoaded_LowPriority;
};
