// Fill out your copyright notice in the Description page of Project Settings.


#include "KitePlayerController.h"

#include "KitePlayerState.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"

AKitePlayerState* AKitePlayerController::GetKitePlayerState() const
{
	return CastChecked<AKitePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UKiteAbilitySystemComponent* AKitePlayerController::GetKiteAbilitySystemComponent() const
{
	const AKitePlayerState* KitePS = GetKitePlayerState();
	return (KitePS ? KitePS->GetKiteAbilitySystemComponent() : nullptr);
}

void AKitePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UKiteAbilitySystemComponent* KiteAsc = GetKiteAbilitySystemComponent())
	{
		KiteAsc->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
