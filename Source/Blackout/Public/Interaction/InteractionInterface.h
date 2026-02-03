// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLACKOUT_API IInteractionInterface
{
	GENERATED_BODY()

	
public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hightlight")
	void Highlight();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hightlight")
	void Unhighlight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsPickable() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsStorable() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Throw(const FVector& ThrowForce);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void PreparePickup();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsFlashLight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	UTexture2D* GetIcon();	
};
