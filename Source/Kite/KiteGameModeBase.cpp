// Copyright Epic Games, Inc. All Rights Reserved.


#include "KiteGameModeBase.h"

AKiteGameModeBase::AKiteGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_KiteCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
