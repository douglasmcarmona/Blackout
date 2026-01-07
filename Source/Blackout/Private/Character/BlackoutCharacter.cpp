// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlackoutCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABlackoutCharacter::ABlackoutCharacter()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->bUsePawnControlRotation = true;

	GrabbedObjectSocket = CreateDefaultSubobject<USceneComponent>("GrabbedObjectSocket");
	GrabbedObjectSocket->SetupAttachment(GetRootComponent());
	GrabbedObjectSocket->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	GrabbedObjectBillboard = CreateDefaultSubobject<UBillboardComponent>("GrabbedObjectBillboard");
	GrabbedObjectBillboard->SetupAttachment(GrabbedObjectSocket);	
}

void ABlackoutCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackoutCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementDirection = InputValue.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementDirection.Y);
	AddMovementInput(Right, MovementDirection.X);
}

void ABlackoutCharacter::LookAround(const FInputActionValue& InputValue)
{
	const FVector2D LookAroundDirection = InputValue.Get<FVector2D>();
	AddControllerYawInput(LookAroundDirection.X);
	AddControllerPitchInput(bInvertY ? LookAroundDirection.Y : -LookAroundDirection.Y);
}

void ABlackoutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlackoutCharacter::Move);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this,  &ABlackoutCharacter::LookAround);
	}
}
