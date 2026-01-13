// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "InteractableActor.generated.h"

#define CUSTOM_DEPTH_HIGHLIGHT_STENCIL_VALUE 250.f;

UCLASS()
class BLACKOUT_API AInteractableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	AInteractableActor();
	virtual void Highlight_Implementation() override;
	virtual void Unhighlight_Implementation() override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void ShowInteractWidget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void HideInteractWidget();

	virtual void ShowInteractWidget_Implementation() {}
	virtual void HideInteractWidget_Implementation() {}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

};
