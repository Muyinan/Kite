// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/KiteAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/KiteGameplayAbility_Combo.h"

UKiteAbilitySystemComponent::UKiteAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UKiteAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		// ActivatableAbilities是ASC里所有已经激活的能力
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UKiteAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UKiteAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			// 重要！！！AbilitySpec->Ability是CDO，GetAbilityInstances()获得的才是技能实例
			if (UGameplayAbility* Ability = AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);

					// Combo技能需要特殊处理
					if(UKiteGameplayAbility_Combo* ComboAbility = Cast<UKiteGameplayAbility_Combo>(Ability))
					{
						ActivatingComboAbilityInputPressed(*AbilitySpec);
					}
				}
				else
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UKiteAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UKiteAbilitySystemComponent::ActivatingComboAbilityInputPressed(FGameplayAbilitySpec& Spec)
{
	// Combo技能有且只有一个实例，所以直接取第一个	
	UKiteGameplayAbility_Combo* ComboAbility = Cast<UKiteGameplayAbility_Combo>(Spec.GetAbilityInstances()[0]);
	if(!IsValid(ComboAbility))
	{
		return;
	}
	
	// 如果没有绑定委托，则绑定
	if (!ComboAbility->ActiveNextComboDelegateCB.IsBound())
	{
		ComboAbility->ActiveNextComboDelegateCB.BindLambda([this, &Spec]()
		{
			// 直接取消当前Combo的后摇并进入下一个Combo
			// 重要！！！对技能的操作要用Spec.Handle和Spec.Ability，不要用实例ComboAbility！！！
			CancelAbility(Spec.Ability);
			TryActivateAbility(Spec.Handle);
		});
	}

	// 后摇中，可以直接进入下一个Combo
	if (ComboAbility->State == EKiteAbilityState::Recovery)
	{
		ComboAbility->TryActiveNextCombo();
	}
	// 暂时还不能进入下一个Combo，这里将输入缓存起来，等委托去处理
	else if(ComboAbility->State == EKiteAbilityState::Attacking)
	{
		ComboAbility->bInputBuffer = true;
	}
	
}