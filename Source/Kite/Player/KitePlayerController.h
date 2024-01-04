// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "KitePlayerController.generated.h"

class AKitePlayerState;
class UKiteAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class KITE_API AKitePlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerController")
	AKitePlayerState* GetKitePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerController")
	UKiteAbilitySystemComponent* GetKiteAbilitySystemComponent() const;
	
	// ~ Begin APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	// ~ End APlayerController interface
};
