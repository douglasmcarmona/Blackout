// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlackoutCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Interaction/InteractionInterface.h"

// Sets default values
ABlackoutCharacter::ABlackoutCharacter()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->bUsePawnControlRotation = true;

	RightHand = CreateDefaultSubobject<USceneComponent>("RightHand");
	RightHand->SetupAttachment(CameraComponent);
	RightHand->SetRelativeLocation(FVector(60.f, 25.f, -20.f));

	LeftHand = CreateDefaultSubobject<USceneComponent>("LeftHand");
	LeftHand->SetupAttachment(CameraComponent);
	LeftHand->SetRelativeLocation(FVector(60.f, -25.f, -20.f));
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

void ABlackoutCharacter::Interact()
{
	if (IsInteractableActor(ThisActor))
	{
		IInteractionInterface::Execute_Interact(ThisActor);
		if (IInteractionInterface::Execute_IsPickable(ThisActor))
		{
			FAttachmentTransformRules AttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false);
			
			switch (GetFreeHand())
			{
				case 0: case 2:
					ThisActor->AttachToComponent(RightHand, AttachmentTransformRules);	
				break;
				case 1:
					ThisActor->AttachToComponent(LeftHand, AttachmentTransformRules);
				break;
				default: break;
			}
		}
	}
}

void ABlackoutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlackoutCharacter::Move);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this,  &ABlackoutCharacter::LookAround);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABlackoutCharacter::Interact);
	}
}

void ABlackoutCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const FVector TraceStart = GetActorLocation() + FVector(0.0f, 0.0f, EyesightZ);
	const FVector TraceEnd = TraceStart + GetControlRotation().Vector() * MaxInteractableDistance;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const FCollisionShape SphereCollision = FCollisionShape::MakeSphere(InteractionRadius);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);	
	
	GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		SphereCollision,
		CollisionParams		
		);

	if (HitResult.bBlockingHit)
	{
		LastActor = ThisActor;
		if (IsInteractableActor(HitResult.GetActor())) ThisActor = HitResult.GetActor();
		else ThisActor = nullptr;

		if (ThisActor != LastActor)
		{
			if (IsInteractableActor(LastActor)){ IInteractionInterface::Execute_Unhighlight(LastActor); }
			if (IsInteractableActor(ThisActor)){ IInteractionInterface::Execute_Highlight(ThisActor); }
		}
	}
	else
	{
		if (IsInteractableActor(LastActor)){ IInteractionInterface::Execute_Unhighlight(LastActor); }
	}	
}

bool ABlackoutCharacter::IsInteractableActor(const AActor* Actor) const
{
	return Actor != nullptr && Actor->Implements<UInteractionInterface>();
}

uint32 ABlackoutCharacter::GetFreeHand() const
{
	return (RightHand->GetNumChildrenComponents() > 0 ? 1 :0) | (LeftHand->GetNumChildrenComponents() > 0 ? 1 : 0) << 1;	
}
