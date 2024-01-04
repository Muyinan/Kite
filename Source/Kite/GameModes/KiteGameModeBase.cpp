// Copyright Epic Games, Inc. All Rights Reserved.


#include "KiteGameModeBase.h"

#include "KiteExperienceDefinition.h"
#include "KiteExperienceManagerComponent.h"
#include "KiteGameState.h"
#include "Character/KiteCharacter.h"
#include "Player/KitePlayerController.h"
#include "Player/KitePlayerState.h"
#include "System/KiteAssetManager.h"

AKiteGameModeBase::AKiteGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AKiteGameState::StaticClass();
	PlayerControllerClass = AKitePlayerController::StaticClass();
	PlayerStateClass = AKitePlayerState::StaticClass();
	DefaultPawnClass = AKiteCharacter::StaticClass();
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_KiteCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

const UKitePawnData* AKiteGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const AKitePlayerState* KitePS = InController->GetPlayerState<AKitePlayerState>())
		{
			if (const UKitePawnData* PawnData = KitePS->GetPawnData<UKitePawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UKiteExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UKiteExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UKiteExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		return UKiteAssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}
