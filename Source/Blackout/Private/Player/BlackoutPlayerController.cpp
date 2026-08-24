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
	SetInputMode(FInputModeGameOnly());	
}

void ABlackoutPlayerController::ChangeMappingContext(const EMappingContext SelectedMappingContext) const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->ClearAllMappings();
		
		switch (SelectedMappingContext)
		{
		case EMappingContext::Default: default:
			if (BlackoutPlayerContext)
			{
				EnhancedInputSubsystem->AddMappingContext(BlackoutPlayerContext, 0);
			}
			return;
			
		case EMappingContext::PauseMenu:
			if (PauseMenuContext)
			{
				EnhancedInputSubsystem->AddMappingContext(PauseMenuContext, 0);				
			}
		}
	}
}
