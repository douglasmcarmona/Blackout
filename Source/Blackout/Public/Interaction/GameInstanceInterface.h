// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to call specific functions on our game instance object
 */
class BLACKOUT_API IGameInstanceInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * Loads the level identified in the map by the given string
	 * @param MapName The map key which relates to the destination map
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TravelToMap(const FString& MapName);
};
