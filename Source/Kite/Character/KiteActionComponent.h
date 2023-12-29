// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "KiteActionComponent.generated.h"

/**
 * 
 */
UCLASS()
class KITE_API UKiteActionComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Kite|Pawn")
	static UKiteActionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UKiteActionComponent>() : nullptr); }
};
