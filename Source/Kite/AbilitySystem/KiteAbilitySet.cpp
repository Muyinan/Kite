// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/KiteAbilitySet.h"

#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "KiteAbilitySystemComponent.h"
#include "Abilities/KiteGameplayAbility.h"
#include "Common/KiteMacros.h"

UKiteAbilitySet::UKiteAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UKiteAbilitySet::GiveToAbilitySystem(UKiteAbilitySystemComponent* KiteASC, FKiteAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
		check(KiteASC);

	if (!KiteASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FKiteAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogKiteAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UKiteGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UKiteGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = KiteASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FKiteAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogKiteAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = KiteASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, KiteASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FKiteAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogKiteAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(KiteASC->GetOwner(), SetToGrant.AttributeSet);
		KiteASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}

void FKiteAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FKiteAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}


void FKiteAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FKiteAbilitySet_GrantedHandles::TakeFromAbilitySystem(UKiteAbilitySystemComponent* KiteASC)
{
	check(KiteASC);

	if (!KiteASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			KiteASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			KiteASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		KiteASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}