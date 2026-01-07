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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<USceneComponent> GrabbedObjectSocket;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> GrabbedObjectBillboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAroundAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bInvertY = false;

private:
	void Move(const FInputActionValue& InputValue);
	void LookAround(const FInputActionValue& InputValue);
};
