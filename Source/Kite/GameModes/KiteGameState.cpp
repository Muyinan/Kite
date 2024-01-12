// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/KiteGameState.h"

#include "KiteExperienceManagerComponent.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"

AKiteGameState::AKiteGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UKiteAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<UKiteExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
	ExperienceManagerComponent->SetIsReplicated(true);
}
