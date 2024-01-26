// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KiteGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API UKiteGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UKiteGameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
