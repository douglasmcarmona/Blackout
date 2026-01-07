// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlackoutPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BLACKOUT_API ABlackoutPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> BlackoutPlayerContext;
};
