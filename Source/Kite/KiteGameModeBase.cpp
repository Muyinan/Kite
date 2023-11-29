// Copyright Epic Games, Inc. All Rights Reserved.


#include "KiteGameModeBase.h"

AKiteGameModeBase::AKiteGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_KiteCharacter"));
	// UE_LOG(LogTemp, Log, TEXT("~1"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		// UE_LOG(LogTemp, Log, TEXT("~2"));
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
