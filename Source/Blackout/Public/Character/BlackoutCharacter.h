// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interaction/HandInterface.h"
#include "BlackoutCharacter.generated.h"

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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual AActor* GetRightHandItem_Implementation() const override;
	virtual AActor* GetLeftHandItem_Implementation() const override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bInvertY = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float EyesightZ = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float MaxInteractableDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> ThisActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> LastActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> RightHandItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> LeftHandItem;
	

private:
	void Move(const FInputActionValue& InputValue);
	void LookAround(const FInputActionValue& InputValue);
	void Interact();
	bool IsInteractableActor(const AActor* Actor) const;
	uint32 GetFreeHand() const;
	void UseItem(const FInputActionValue& InputActionValue);
};
