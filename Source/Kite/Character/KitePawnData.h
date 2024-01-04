// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KitePawnData.generated.h"


/**
 * 
 */
UCLASS()
class KITE_API UKitePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UKitePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	
	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kite|Abilities")
	TArray<TObjectPtr<UKiteAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kite|Abilities")
	TObjectPtr<UKiteAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kite|Input")
	TObjectPtr<UKiteInputConfig> InputConfig;
};
