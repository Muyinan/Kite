// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KiteAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API UKiteAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 防止在character PossessedBy()时重复添加AbilitySet
	bool bCharacterAbilitiesGiven = false;
	
public:

	UKiteAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	// 正在激活中的Combo技能被按下了
	void ActivatingComboAbilityInputPressed(FGameplayAbilitySpec& Spec);
	
protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// // Combo技能的输入缓存，因为Combo技能需要自己处理输入缓存，这里和普通技能分开
	// TArray<FGameplayAbilitySpecHandle> ComboInputPressedSpecHandles;
};