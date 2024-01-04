// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "KitePlayerState.generated.h"

class UKitePawnData;
class UKiteAbilitySystemComponent;
/**
 * 
 */
UCLASS(Config = Game)
class KITE_API AKitePlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AKitePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Kite|PlayerState")
	UKiteAbilitySystemComponent* GetKiteAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UKitePawnData* InPawnData);

	static const FName NAME_KiteAbilityReady;

protected:
	
	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const UKiteExperienceDefinition* CurrentExperience);
	
protected:

	// TODO，不注释掉会编译报错
	// UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	UPROPERTY()
	TObjectPtr<const UKitePawnData> PawnData;
	
private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Kite|PlayerState")
	TObjectPtr<UKiteAbilitySystemComponent> AbilitySystemComponent;
};
