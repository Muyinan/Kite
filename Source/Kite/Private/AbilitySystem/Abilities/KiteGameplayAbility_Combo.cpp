// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem\Abilities\KiteGameplayAbility_Combo.h"

#include "Common/KiteMacros.h"


void UKiteGameplayAbility_Combo::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
}

void UKiteGameplayAbility_Combo::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
}

// 类似BeginPlay()
void UKiteGameplayAbility_Combo::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	AbilityStateChangedDelegate.AddUObject(this, &ThisClass::OnAbilityStateChanged);
	AbilityStateChangedDelegate.AddUObject(this, &ThisClass::K2_OnAbilityStateChanged);
}

void UKiteGameplayAbility_Combo::OnAbilityStateChanged(EKiteAbilityState OldState, EKiteAbilityState NewState)
{
	// 进入攻击后摇，检查是否存在输入缓存，如果存在则直接激活
	if (OldState == EKiteAbilityState::Attacking && NewState == EKiteAbilityState::Recovery)
	{
		if (bInputBuffer)
		{
			TryActiveNextCombo();
		}
	}
}

void UKiteGameplayAbility_Combo::TryActiveNextCombo()
{
	ActiveNextComboDelegateCB.ExecuteIfBound();
	
	bInputBuffer = false;
}

void UKiteGameplayAbility_Combo::SetAndBroadCastStateChanged(EKiteAbilityState NewState)
{
	EKiteAbilityState OldState = State;
	State = NewState;
	if(OldState != NewState)
	{
		BroadcastAbilityStateChanged(OldState, NewState);
	}
}
