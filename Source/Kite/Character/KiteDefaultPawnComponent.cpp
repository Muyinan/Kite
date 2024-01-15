// Fill out your copyright notice in the Description page of Project Settings.


#include "KiteDefaultPawnComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "KitePawnData.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"
#include "Common/KiteGameplayTags.h"
#include "Common/KiteMacros.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/KiteInputComponent.h"
#include "Input/KiteInputConfig.h"
#include "Net/UnrealNetwork.h"
#include "Player/KiteLocalPlayer.h"

// gamefeature加载接口
// UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin();
const FName UKiteDefaultPawnComponent::NAME_BindInputsNow("BindInputsNow");
const FName UKiteDefaultPawnComponent::NAME_ActorFeatureName("Hero");

UKiteDefaultPawnComponent::UKiteDefaultPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
	AbilitySystemComponent = nullptr;
	bReadyToBindInputs = false;
}

void UKiteDefaultPawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void UKiteDefaultPawnComponent::OnRep_PawnData()
{
	
}

void UKiteDefaultPawnComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void UKiteDefaultPawnComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

void UKiteDefaultPawnComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	// if (AKitePlayerController* KiteController = Cast<AKitePlayerController>(Controller))
	// {
	// 	KiteController->SetIsAutoRunning(false);
	// }
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UKiteDefaultPawnComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UKiteDefaultPawnComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
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

	if (const UKiteInputConfig* InputConfig = PawnData->InputConfig)
	{
		if (!SPtr_DefaultMappingContext.IsNull())
		{
			if (!SPtr_DefaultMappingContext.IsValid())
			{
				SPtr_DefaultMappingContext.ToSoftObjectPath().TryLoad();
			}
			Subsystem->AddMappingContext(SPtr_DefaultMappingContext.Get(), 0);
		}
	
		// The Kite Input Component has some additional functions to map Gameplay Tags to an Input Action.
		// If you want this functionality but still want to change your input component class, make it a subclass
		// of the UKiteInputComponent or modify this component accordingly.
		UKiteInputComponent* KiteIC = Cast<UKiteInputComponent>(PlayerInputComponent);
		if (ensureMsgf(KiteIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UKiteInputComponent or a subclass of it.")))
		{
			// Add the key mappings that may have been set by the player
			KiteIC->AddInputMappings(InputConfig, Subsystem);
	
			// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
			// be triggered directly by these input actions Triggered events. 
			TArray<uint32> BindHandles;
			KiteIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	
			KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
			KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
			// KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
			// KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
			// KiteIC->BindNativeAction(InputConfig, KiteGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
		}
	}
	
	
	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UKiteDefaultPawnComponent::SetPawnData(const UKitePawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (!Pawn->IsLocallyControlled())
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogKite, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	// CheckDefaultInitialization();
}
