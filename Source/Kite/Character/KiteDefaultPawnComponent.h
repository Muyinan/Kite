// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "KiteDefaultPawnComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class KITE_API UKiteDefaultPawnComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	UKiteDefaultPawnComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Kite|Pawn")
	static UKiteDefaultPawnComponent* FindDefaultPawnComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UKiteDefaultPawnComponent>() : nullptr); }

	void SetPawnData(const class UKitePawnData* InPawnData);

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;
	
protected:
	UFUNCTION()
	void OnRep_PawnData();

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const struct FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	
protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UInputMappingContext> SPtr_DefaultMappingContext;
	
	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Kite|Pawn")
	TObjectPtr<const UKitePawnData> PawnData;

	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY()
	TObjectPtr<class UKiteAbilitySystemComponent> AbilitySystemComponent;

	/** True when player input bindings have been applied, will never be true for non - players */
	bool bReadyToBindInputs;
};
