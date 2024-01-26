// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KitePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API AKitePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//~Controller interface
	virtual void InitPlayerState() override;

	//~PlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;


	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerController")
	AKitePlayerState* GetKitePlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerController")
	UKiteAbilitySystemComponent* GetKiteAbilitySystemComponent() const;
};
