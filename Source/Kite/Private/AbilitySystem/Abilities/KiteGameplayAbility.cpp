// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KiteGameplayAbility.h"

UKiteGameplayAbility::UKiteGameplayAbility()
{
	
}

void UKiteGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
}

void UKiteGameplayAbility::BroadcastAbilityStateChanged(EKiteAbilityState OldState, EKiteAbilityState NewState)
{
	AbilityStateChangedDelegate.Broadcast(OldState, NewState);
	K2_AbilityStateChangedDelegate.Broadcast(OldState, NewState);
}

