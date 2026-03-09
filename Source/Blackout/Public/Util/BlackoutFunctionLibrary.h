// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlackoutFunctionLibrary.generated.h"

class UPaperNoteInfo;
/**
 * 
 */
UCLASS()
class BLACKOUT_API UBlackoutFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Blackout|Data", meta = (DefaultToSelf = "WorldContextObject"))
	static UPaperNoteInfo* GetPaperNoteInfo(const UObject* WorldContextObject);	
};
