// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interaction/HandInterface.h"
#include "BlackoutCharacter.generated.h"

class UInventoryComponent;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UBillboardComponent;

UCLASS()
class BLACKOUT_API ABlackoutCharacter : public ACharacter, public IHandInterface
{
	GENERATED_BODY()

public:	
	ABlackoutCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual AActor* GetRightHandItem_Implementation() const override;
	virtual AActor* GetLeftHandItem_Implementation() const override;
	virtual void SetRightHandItem_Implementation(AActor* Item) override;
	virtual void SetLeftHandItem_Implementation(AActor* Item) override;
	virtual bool IsHandHoldingItem_Implementation(const bool bIsRightHand) const override;
	virtual FVector GetHandLocation_Implementation(const bool bIsRightHand) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsInventoryOpen = false;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<USceneComponent> RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<USceneComponent> LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAroundAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> UseItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> EnableThrowAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ToggleInventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bInvertY = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float EyesightZ = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float MaxInteractableDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> ThisActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> LastActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> RightHandItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> LeftHandItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	

private:
	void Move(const FInputActionValue& InputValue);
	void LookAround(const FInputActionValue& InputValue);
	void Interact();
	bool IsInteractableActor(const AActor* Actor) const;
	uint32 GetFreeHand() const;	
	void UseItem(const FInputActionValue& InputActionValue);
	void EnableThrow(const FInputActionValue& InputActionValue);
	void ToggleInventory();
	
	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsThrowEnabled = false;
};
