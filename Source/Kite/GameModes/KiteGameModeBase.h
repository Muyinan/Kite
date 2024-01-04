// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "KiteGameModeBase.generated.h"

class UKitePawnData;
/**
 * 
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class KITE_API AKiteGameModeBase : public AModularGameModeBase
{
	GENERATED_BODY()
public:
	AKiteGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Kite|Pawn")
	const UKitePawnData* GetPawnDataForController(const AController* InController) const;

	
};
