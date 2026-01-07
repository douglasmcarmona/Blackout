// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BlackoutPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ABlackoutPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputSystem && BlackoutPlayerContext)
	{
		EnhancedInputSystem->AddMappingContext(BlackoutPlayerContext, 0);
	}
}
