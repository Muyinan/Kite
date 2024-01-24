// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/KiteInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(KiteInputComponent)

UKiteInputComponent::UKiteInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UKiteInputComponent::AddInputMappings(const UKiteInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UKiteInputComponent::RemoveInputMappings(const UKiteInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UKiteInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}