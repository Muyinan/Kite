// Copyright Epic Games, Inc. All Rights Reserved.


#include "KiteGameModeBase.h"

#include "KiteExperienceDefinition.h"
#include "KiteExperienceManagerComponent.h"
#include "KiteGameState.h"
#include "KiteWorldSettings.h"
#include "Character/KiteCharacter.h"
#include "Character/KiteDefaultPawnComponent.h"
#include "Common/KiteMacros.h"
#include "Kismet/GameplayStatics.h"
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
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/B_KiteCharacter"));
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

void AKiteGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Wait for the next frame to give time to initialize startup settings
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignment);
	// HandleMatchAssignment();
}

APawn* AKiteGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UKiteDefaultPawnComponent* DefaultPawnComp = UKiteDefaultPawnComponent::FindDefaultPawnComponent(SpawnedPawn))
			{
				if (const UKitePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					DefaultPawnComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogKite, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogKite, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogKite, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

void AKiteGameModeBase::HandleMatchAssignment()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// Precedence order (highest wins)
	//  - Matchmaking assignment (if present)
	//  - URL Options override
	//  - Developer Settings (PIE only)
	//  - Command Line override
	//  - World Settings
	//  - Dedicated server
	//  - Default experience

	UWorld* World = GetWorld();

	// if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	// {
	// 	const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
	// 	ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UKiteExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	// 	ExperienceIdSource = TEXT("OptionsString");
	// }
	//
	// if (!ExperienceId.IsValid() && World->IsPlayInEditor())
	// {
	// 	ExperienceId = GetDefault<UKiteDeveloperSettings>()->ExperienceOverride;
	// 	ExperienceIdSource = TEXT("DeveloperSettings");
	// }

	// see if the command line wants to set the experience
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UKiteExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	// see if the world settings has a default experience
	if (!ExperienceId.IsValid())
	{
		if (AKiteWorldSettings* TypedWorldSettings = Cast<AKiteWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	UKiteAssetManager& AssetManager = UKiteAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogKiteExperience, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	if (!ExperienceId.IsValid())
	{
		// if (TryDedicatedServerLogin())
		// {
		// 	// This will start to host as a dedicated server
		// 	return;
		// }

		//@TODO: Pull this from a config setting or something
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("KiteExperienceDefinition"), FName("B_KiteDefaultExperience"));
		ExperienceIdSource = TEXT("Default");
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AKiteGameModeBase::OnMatchAssignmentGiven(const FPrimaryAssetId& ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogKiteExperience, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

			UKiteExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UKiteExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogKiteExperience, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}