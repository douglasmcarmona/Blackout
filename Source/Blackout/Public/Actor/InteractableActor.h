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
	virtual void Interact_Implementation() override;
	virtual bool IsPickable_Implementation() const override;
	virtual bool IsStorable_Implementation() const override;
	virtual bool IsThrowable_Implementation() const override;
	virtual void Throw_Implementation(const FVector& ThrowForce) override;
	virtual void PreparePickup_Implementation() override;
	virtual UTexture2D* GetIcon_Implementation() override;
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void ShowInteractWidget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void HideInteractWidget();

	virtual void ShowInteractWidget_Implementation() {}
	virtual void HideInteractWidget_Implementation() {}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPickable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsStorable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsThrowable = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> InventoryIcon;
};
