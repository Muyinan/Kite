// Fill out your copyright notice in the Description page of Project Settings.


#include "System/KiteAssetManager.h"

#include "Common/KiteMacros.h"

UKiteAssetManager::UKiteAssetManager()
{
}

UKiteAssetManager& UKiteAssetManager::Get()
{
	check(GEngine);

	if (UKiteAssetManager* Singleton = Cast<UKiteAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogKite, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to KiteAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UKiteAssetManager>();
}

const UKitePawnData* UKiteAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}

UObject* UKiteAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UKiteAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UKiteAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
