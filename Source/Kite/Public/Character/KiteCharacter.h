// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "KiteCharacter.generated.h"

UCLASS(config=Game)
class KITE_API AKiteCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kite|Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UKiteInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kite|Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, Category = "Kite|Ability")
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	// Sets default values for this character's properties
	AKiteCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for looking input */
	void Input_LookMouse(const FInputActionValue& Value);
	
	/** Called for movement input */
	void Input_Move(const FInputActionValue& Value);
};
