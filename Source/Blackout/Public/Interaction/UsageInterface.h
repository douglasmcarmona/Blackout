// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsageInterface : public UInterface
{
	GENERATED_BODY()
};


class BLACKOUT_API IUsageInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usage")
	bool CanBeUsed(bool bIsRightHand);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usage")
	void Use();
};
