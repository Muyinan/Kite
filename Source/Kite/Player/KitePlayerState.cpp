// Fill out your copyright notice in the Description page of Project Settings.


#include "KitePlayerState.h"

#include "AbilitySystem/KiteAbilitySet.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"
#include "Character/KitePawnData.h"
#include "Common/KiteMacros.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameModes/KiteExperienceManagerComponent.h"
#include "GameModes/KiteGameModeBase.h"
#include "Net/UnrealNetwork.h"

const FName AKitePlayerState::NAME_KiteAbilityReady("KiteAbilitiesReady");

AKitePlayerState::AKitePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 实例化ASC
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UKiteAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
}

UAbilitySystemComponent* AKitePlayerState::GetAbilitySystemComponent() const
{
	return GetKiteAbilitySystemComponent();
}

void AKitePlayerState::PostInitializeComponents()
{
	kwarn("PostInitializeComponents");
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UKiteExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UKiteExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnKiteExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void AKitePlayerState::SetPawnData(const UKitePawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogKite, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	// 清除旧PawnData，编译会报错，先注释
	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	
	PawnData = InPawnData;

	// for (const UKiteAbilitySet* AbilitySet : PawnData->AbilitySets)
	// {
	// 	if (AbilitySet)
	// 	{
	// 		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
	// 	}
	// }

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_KiteAbilityReady);
	
	ForceNetUpdate();
}

void AKitePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void AKitePlayerState::OnRep_PawnData()
{
}

void AKitePlayerState::OnExperienceLoaded(const UKiteExperienceDefinition* CurrentExperience)
{
	kwarn("OnExperienceLoaded");
	if (AKiteGameModeBase* KiteGameMode = GetWorld()->GetAuthGameMode<AKiteGameModeBase>())
	{
		if (const UKitePawnData* NewPawnData = KiteGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogKite, Error, TEXT("AKitePlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}
