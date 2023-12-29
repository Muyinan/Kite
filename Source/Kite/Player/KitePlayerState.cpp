// Fill out your copyright notice in the Description page of Project Settings.


#include "KitePlayerState.h"

#include "AbilitySystem/KiteAbilitySystemComponent.h"

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
