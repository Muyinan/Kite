#pragma once

#include "KiteAbilityTypes.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct KITE_API FKiteActionAbilityData
{
	GENERATED_BODY()

	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};

// USTRUCT(BlueprintType)
// struct FKiteAbilityKey
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	TObjectPtr<UInputAction> InputAction;
//
// 	UPROPERTY()
// 	TObjectPtr<UGameplayAbility> GameplayAbility;
//
// 	bool operator==(const FKiteAbilityKey& Other) const
// 	{
// 		return InputAction == Other.InputAction && GameplayAbility == Other.GameplayAbility;
// 	}
//
// 	friend uint32 GetTypeHash(const FKiteAbilityKey& Key)
// 	{
// 		uint32 Hash = 0;
// 		Hash = HashCombine(Hash, GetTypeHash(Key.InputAction));
// 		Hash = HashCombine(Hash, GetTypeHash(Key.GameplayAbility));
// 		return Hash;
// 	}
// };
//
// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameplayAbilities)
// TMap<FKiteAbilityKey, FKiteActionAbilityData*> AbilityMap;