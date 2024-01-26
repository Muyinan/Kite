// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "KiteCharacterHero.generated.h"

UCLASS(config=Game)
class KITE_API AKiteCharacterHero : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kite|Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UKiteInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kite|Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kite|Ability")
	TObjectPtr<class UKiteAbilitySet> AbilitySet;

	UPROPERTY(VisibleAnywhere, Category = "Kite|Ability")
	TObjectPtr<class UKiteAbilitySystemComponent> AbilitySystemComponent;
	
protected:
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:

	AKiteCharacterHero();
	
	virtual void Tick(float DeltaTime) override;

	//~Pawn Interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//** Server only */
	virtual void PossessedBy(AController* NewController) override;
	//** Client only */
	virtual void OnRep_PlayerState() override;
	//~End Pawn Interface
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Input_LookMouse(const FInputActionValue& Value);
	void Input_Move(const FInputActionValue& Value);
	void Input_AbilityInputTagPressed(struct FGameplayTag AbilityInputTag);
	void Input_AbilityInputTagReleased(FGameplayTag AbilityInputTag);

	void InitAbilitySystem();
};
