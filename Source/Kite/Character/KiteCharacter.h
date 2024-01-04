// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "KiteCharacter.generated.h"

class UKiteAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UKiteActionComponent;

UCLASS(config=Game)
class KITE_API AKiteCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKiteCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	
	void MeleeAttack(const FInputActionValue& Value);

	// UFUNCTION()
	// void OnRep_CurrentHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_MeleeAttack(const FInputActionValue& Value);
	
public:

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilities)
	// TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
protected:
	// UPROPERTY(EditDefaultsOnly, Category = "Health")
	// float MaxHealth;
	//
	// UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	// float CurrentHealth;
	
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameplayAbilities)
	// TMap<TPair<TObjectPtr<UInputAction>, TObjectPtr<UGameplayAbility>>, FKiteActionAbilityData*> AbilityMap;
	// TMap<FString, FKiteActionAbilityData> AbilityMap;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MeleeAttackAction;

	UPROPERTY()
	TObjectPtr<UKiteActionComponent> KiteActionComponent;
};
