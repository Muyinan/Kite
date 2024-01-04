// Fill out your copyright notice in the Description page of Project Settings.


#include "KiteActionComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Input/KiteInputConfig.h"
#include "Player/KiteLocalPlayer.h"

// gamefeature加载接口
// UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin();

void UKiteActionComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);
	
	const UKiteLocalPlayer* LP = Cast<UKiteLocalPlayer>(PC->GetLocalPlayer());
	check(LP);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();

	// if (const UKiteInputConfig* InputConfig = PawnData->InputConfig)
	// {
	// 	for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
	// 	{
	// 		if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
	// 		{
	// 			if (Mapping.bRegisterWithSettings)
	// 			{
	// 				if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
	// 				{
	// 					Settings->RegisterInputMappingContext(IMC);
	// 				}
	// 				
	// 				FModifyContextOptions Options = {};
	// 				Options.bIgnoreAllPressedKeysUntilRelease = false;
	// 				// Actually add the config to the local player							
	// 				Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
	// 			}
	// 		}
	// 	}
	//
	// 	// The Kite Input Component has some additional functions to map Gameplay Tags to an Input Action.
	// 	// If you want this functionality but still want to change your input component class, make it a subclass
	// 	// of the UKiteInputComponent or modify this component accordingly.
	// 	UKiteInputComponent* KiteIC = Cast<UKiteInputComponent>(PlayerInputComponent);
	// 	if (ensureMsgf(KiteIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UKiteInputComponent or a subclass of it.")))
	// 	{
	// 		// Add the key mappings that may have been set by the player
	// 		KiteIC->AddInputMappings(InputConfig, Subsystem);
	//
	// 		// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
	// 		// be triggered directly by these input actions Triggered events. 
	// 		TArray<uint32> BindHandles;
	// 		KiteIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	//
	// 		KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
	// 		KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
	// 		KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
	// 		KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
	// 		KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
	// 	}
	// }
	//
	//
	// if (ensure(!bReadyToBindInputs))
	// {
	// 	bReadyToBindInputs = true;
	// }
 //
	// UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	// UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}
