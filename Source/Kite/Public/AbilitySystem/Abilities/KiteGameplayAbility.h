// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KiteGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EKiteAbilityState : uint8
{
	Idle,			// 空闲
	Preparation,	// 攻击前摇
	Attacking,		// 攻击中
	Recovery,		// 攻击后摇
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FAilityStateChangedDelegate, EKiteAbilityState, EKiteAbilityState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAilityStateChangedDelegateDynamic, EKiteAbilityState, OldState, EKiteAbilityState, NewState);

/**
 * 
 */
UCLASS()
class KITE_API UKiteGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kite")
	EKiteAbilityState State = EKiteAbilityState::Idle;
	
	FAilityStateChangedDelegate AbilityStateChangedDelegate;

protected:
	
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "AbilityStateChangedDelegate", AllowPrivateAccess = true))
	FAilityStateChangedDelegateDynamic K2_AbilityStateChangedDelegate;
	
public:
	
	UKiteGameplayAbility();

	//~GameplayAbility Interface
	// 里面可以加类似于BeginPlay()的逻辑
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void BroadcastAbilityStateChanged(EKiteAbilityState OldState, EKiteAbilityState NewState);
};
