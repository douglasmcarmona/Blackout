// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BlackoutHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/Widget/InventoryUserWidget.h"

void ABlackoutHUD::BeginPlay()
{
	Super::BeginPlay();
	TogglePauseButton(true);
	TogglePauseMenu(false);
}

UInventoryWidgetController* ABlackoutHUD::GetInventoryWidgetController(UInventoryComponent* InventoryComponent)
{
	if (!InventoryWidgetController)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetInventoryComponent(InventoryComponent);
	}
	return InventoryWidgetController;
}

void ABlackoutHUD::InitInventoryWidget(UInventoryComponent* InventoryComponent)
{
	checkf(InventoryWidgetControllerClass, TEXT("InventoryWidgetControllerClass uninitialized. Please set it in HUD BP"));
	checkf(InventoryWidgetClass, TEXT("InventoryWidgetClass uninitialized. Please set it in HUD BP"));

	if (!InventoryWidget)
	{
		UUserWidget* UserWidget = CreateWidget<UInventoryUserWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget = Cast<UInventoryUserWidget>(UserWidget);		
	}

	InventoryWidget->AddToViewport();
	InventoryWidgetController = GetInventoryWidgetController(InventoryComponent);
	InventoryWidget->SetInventoryWidgetController(InventoryWidgetController);
	InventoryWidgetController->LoadInventory();
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayerController(), InventoryWidget);
	GetOwningPlayerController()->SetShowMouseCursor(true);
}

void ABlackoutHUD::CloseInventoryWidget() const
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
}

void ABlackoutHUD::TogglePauseButton(const bool bVisible)
{
	if (!PauseButtonWidget && bVisible)
	{
		checkf(PauseButtonWidgetClass, TEXT("PauseButtonWidgetClass uninitialized. Please set it in HUD BP"));
		PauseButtonWidget = CreateWidget(PlayerOwner, PauseButtonWidgetClass);
	}	
	
	if (bVisible)
	{
		PauseButtonWidget->AddToViewport();	
	}
	else if (PauseButtonWidget)
	{
		PauseButtonWidget->RemoveFromParent();	
	}
}

void ABlackoutHUD::TogglePauseMenu(const bool bVisible)
{
	if (!PauseMenuWidget && bVisible)
	{
		checkf(PauseMenuWidgetClass, TEXT("PauseMenuWidgetClass uninitialized. Please set it in HUD BP"));
		PauseMenuWidget = CreateWidget(PlayerOwner, PauseMenuWidgetClass);
	}
	
	if (bVisible)
	{
		PauseMenuWidget->AddToViewport();
	}
	else if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
	}
}
