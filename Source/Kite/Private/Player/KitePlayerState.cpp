// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitePlayerState.h"

#include "AbilitySystem/KiteAbilitySystemComponent.h"

AKitePlayerState::AKitePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKiteAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AKitePlayerState::GetAbilitySystemComponent() const 
{
	return AbilitySystemComponent;
}
