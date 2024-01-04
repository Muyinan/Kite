// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/KiteSettingsLocal.h"

UKiteSettingsLocal::UKiteSettingsLocal()
{
}

UKiteSettingsLocal* UKiteSettingsLocal::Get()
{
	return GEngine ? CastChecked<UKiteSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void UKiteSettingsLocal::OnExperienceLoaded()
{
}
