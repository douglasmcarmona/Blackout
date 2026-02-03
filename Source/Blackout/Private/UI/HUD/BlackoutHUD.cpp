// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BlackoutHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/Widget/InventoryUserWidget.h"

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
	checkf(InventoryWidgetControllerClass, TEXT("InventoryWidgetControllerClass uninitialized. Please fill in HUD BP"));
	checkf(InventoryWidgetClass, TEXT("InventoryWidgetClass uninitialized. Please fill in HUD BP"));

	UUserWidget* UserWidget = CreateWidget<UInventoryUserWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget = Cast<UInventoryUserWidget>(UserWidget);
	
	InventoryWidget->SetInventoryWidgetController(GetInventoryWidgetController(InventoryComponent));
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayerController(), InventoryWidget);
	GetOwningPlayerController()->SetShowMouseCursor(true);
	InventoryWidget->AddToViewport();
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
