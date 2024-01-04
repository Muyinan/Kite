// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "KiteSettingsLocal.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API UKiteSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UKiteSettingsLocal();

	static UKiteSettingsLocal* Get();

	void OnExperienceLoaded();

};
