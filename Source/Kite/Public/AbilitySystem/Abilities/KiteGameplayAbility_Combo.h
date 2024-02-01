// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/KiteGameplayAbility.h"
#include "KiteGameplayAbility_Combo.generated.h"

DECLARE_DELEGATE(FActiveNextComboDelegateCB)

/**
 *	
 */
USTRUCT(BlueprintType)
struct FAbilityComboInfo
{
	GENERATED_BODY()

public:
	// 当前combo需要播放的蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage = nullptr;

	// // 当前ComboInfo的索引
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// int CurrentComboIndex = -1;
	
	// 动画伤害
};

/**
 * 
 */
UCLASS()
class KITE_API UKiteGameplayAbility_Combo : public UKiteGameplayAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kite|Combo")
	TArray<FAbilityComboInfo> ComboInfoList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kite|Combo")
	int CurComboIndex = -1;
	
	FActiveNextComboDelegateCB ActiveNextComboDelegateCB;

	// 输入缓存，值为true时自动进入下一个Combo
	bool bInputBuffer = false;
	
public:
	//~GameplayAbility Interface
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr);
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;// 里面可以加类似于BeginPlay()的逻辑
	
	virtual void OnAbilityStateChanged(EKiteAbilityState OldState, EKiteAbilityState NewState);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnAbilityStateChanged"), Category = "Kite|Ability")
	void K2_OnAbilityStateChanged(EKiteAbilityState OldState, EKiteAbilityState NewState);

	UFUNCTION(BlueprintCallable, Category = "Kite|Ability")
	void TryActiveNextCombo();

	UFUNCTION(BlueprintCallable, Category = "Kite|Ability")
	void SetAndBroadCastStateChanged(EKiteAbilityState NewState);
};
