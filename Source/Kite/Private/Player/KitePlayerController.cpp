// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitePlayerController.h"

#include "AbilitySystem/KiteAbilitySystemComponent.h"
#include "Player/KitePlayerState.h"

// 只有服务器上会调用该函数
void AKitePlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	
}

void AKitePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UKiteAbilitySystemComponent* KiteASC = GetKiteAbilitySystemComponent())
	{
		KiteASC->ProcessAbilityInput(DeltaTime, bGamePaused);	
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

AKitePlayerState* AKitePlayerController::GetKitePlayerState() const
{
	return CastChecked<AKitePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UKiteAbilitySystemComponent* AKitePlayerController::GetKiteAbilitySystemComponent() const
{
	const AKitePlayerState* KitePS = GetKitePlayerState();
	return (KitePS ? KitePS->GetKiteAbilitySystemComponent() : nullptr);
}


