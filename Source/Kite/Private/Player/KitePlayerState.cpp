// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitePlayerState.h"

#include "AbilitySystem/KiteAbilitySystemComponent.h"

AKitePlayerState::AKitePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKiteAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// ** playerstate的默认更新频率非常低，当ASC挂在PlayerState上时需要调整网络更新频率，非常重要！不然可能服务器上ASC发生了改变，但是更新频率低导致客户端更新不到。比如播放蒙太奇等 *//
	NetUpdateFrequency = 30.0f;
}

UAbilitySystemComponent* AKitePlayerState::GetAbilitySystemComponent() const 
{
	return GetKiteAbilitySystemComponent();
}

void AKitePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
}
