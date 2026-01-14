// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlackoutCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UBillboardComponent;

UCLASS()
class BLACKOUT_API ABlackoutCharacter : public ACharacter
{
	GENERATED_BODY()

public:	
	ABlackoutCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

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
	

private:
	void Move(const FInputActionValue& InputValue);
	void LookAround(const FInputActionValue& InputValue);
	void Interact();
	bool IsInteractableActor(const AActor* Actor) const;
	uint32 GetFreeHand() const;
};
