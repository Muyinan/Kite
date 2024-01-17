// Fill out your copyright notice in the Description page of Project Settings.


#include "KitePlayerController.h"

#include "KitePlayerState.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"
#include "Character/KiteCharacter.h"
#include "Character/KiteDefaultPawnComponent.h"

AKitePlayerState* AKitePlayerController::GetKitePlayerState() const
{
	return CastChecked<AKitePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UKiteAbilitySystemComponent* AKitePlayerController::GetKiteAbilitySystemComponentInPS() const
{
	const AKitePlayerState* KitePS = GetKitePlayerState();
	return (KitePS ? KitePS->GetKiteAbilitySystemComponent() : nullptr);
}

UKiteAbilitySystemComponent* AKitePlayerController::GetKiteAbilitySystemComponentInDPC() const
{
	if (AKiteCharacter* KiteCharacter = GetPawn<AKiteCharacter>())
	{
		if(UKiteDefaultPawnComponent* KiteDPC = KiteCharacter->GetComponentByClass<UKiteDefaultPawnComponent>())
		{
			return KiteDPC->GetKiteAbilitySystemComponent();
		}
	}

	return nullptr;
}

void AKitePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UKiteAbilitySystemComponent* KiteAsc = GetKiteAbilitySystemComponentInDPC())
	{
		KiteAsc->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
