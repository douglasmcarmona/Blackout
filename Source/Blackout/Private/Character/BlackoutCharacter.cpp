// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlackoutCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Component/InventoryComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/UsageInterface.h"
#include "Player/BlackoutPlayerController.h"
#include "UI/HUD/BlackoutHUD.h"

// Sets default values
ABlackoutCharacter::ABlackoutCharacter()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, EyesightZ));
	CameraComponent->bUsePawnControlRotation = true;

	RightHand = CreateDefaultSubobject<USceneComponent>("RightHand");
	RightHand->SetupAttachment(CameraComponent);
	RightHand->SetRelativeLocation(FVector(60.f, 25.f, -20.f));

	LeftHand = CreateDefaultSubobject<USceneComponent>("LeftHand");
	LeftHand->SetupAttachment(CameraComponent);
	LeftHand->SetRelativeLocation(FVector(60.f, -25.f, -20.f));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}

void ABlackoutCharacter::BeginPlay()
{
	Super::BeginPlay();
	checkf(MoveAction, TEXT("Please fill in MoveAction"));
	checkf(LookAroundAction, TEXT("Please fill in LookAroundAction"));
	checkf(InteractAction, TEXT("Please fill in InteractAction"));
	checkf(UseItemAction, TEXT("Please fill in UseItemAction"));
	checkf(EnableThrowAction, TEXT("Please fill in EnableThrowAction"));
	checkf(ToggleInventoryAction, TEXT("Please fill in ToggleInventoryAction"));

	InventoryComponent->OnItemStored.AddLambda([this](const FSlot& InSlot, const bool bIsRightHand)
	{	
		if (bIsRightHand)
		{
			RightHandItem->Destroy();
		}
		else
		{
			LeftHandItem->Destroy();
		}
	});
}

void ABlackoutCharacter::Move(const FInputActionValue& InputValue)
{
	if (bIsInventoryOpen) return;
	
	const FVector2D MovementDirection = InputValue.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementDirection.Y);
	AddMovementInput(Right, MovementDirection.X);
}

void ABlackoutCharacter::LookAround(const FInputActionValue& InputValue)
{
	if (bIsInventoryOpen) return;
	
	const FVector2D LookAroundDirection = InputValue.Get<FVector2D>();
	AddControllerYawInput(LookAroundDirection.X);
	AddControllerPitchInput(bInvertY ? LookAroundDirection.Y : -LookAroundDirection.Y);
}

void ABlackoutCharacter::Interact()
{
	if (bIsInventoryOpen) return;
	
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
					IInteractionInterface::Execute_PreparePickup(ThisActor);
					ThisActor->AttachToComponent(RightHand, AttachmentTransformRules);
					RightHandItem = ThisActor;
				break;
				case 1:
					IInteractionInterface::Execute_PreparePickup(ThisActor);
					ThisActor->AttachToComponent(LeftHand, AttachmentTransformRules);
					LeftHandItem = ThisActor;
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
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &ABlackoutCharacter::UseItem);				
		EnhancedInputComponent->BindAction(EnableThrowAction, ETriggerEvent::Triggered, this, &ABlackoutCharacter::EnableThrow);
		EnhancedInputComponent->BindAction(EnableThrowAction, ETriggerEvent::Completed, this,  &ABlackoutCharacter::EnableThrow);
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Completed, this,  &ABlackoutCharacter::ToggleInventory);
	}
}

void ABlackoutCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsInventoryOpen) return;
	
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

AActor* ABlackoutCharacter::GetRightHandItem_Implementation() const
{
	return RightHandItem;
}

AActor* ABlackoutCharacter::GetLeftHandItem_Implementation() const
{	
	return LeftHandItem;
}

bool ABlackoutCharacter::SetRightHandItem_Implementation(AActor* Item)
{
	if (IsHandHoldingItem_Implementation(true)) return false;
	
	FAttachmentTransformRules AttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false);
	
	IInteractionInterface::Execute_PreparePickup(Item);
	Item->AttachToComponent(RightHand, AttachmentTransformRules);
	RightHandItem = Item;
	return true;
}

bool ABlackoutCharacter::SetLeftHandItem_Implementation(AActor* Item)
{
	if (IsHandHoldingItem_Implementation(false)) return false;
	
	FAttachmentTransformRules AttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				false);
	
	IInteractionInterface::Execute_PreparePickup(Item);
	Item->AttachToComponent(LeftHand, AttachmentTransformRules);
	LeftHandItem = Item;
	return true;
}

uint32 ABlackoutCharacter::GetFreeHand() const
{
	return (RightHand->GetNumChildrenComponents() > 0 ? 1 :0) | (LeftHand->GetNumChildrenComponents() > 0 ? 1 : 0) << 1;	
}

bool ABlackoutCharacter::IsHandHoldingItem_Implementation(const bool bIsRightHand) const
{
	const bool bRightHandBusy = RightHandItem != nullptr;
	const bool bLeftHandBusy = LeftHandItem != nullptr;

	return (bIsRightHand && bRightHandBusy) || (!bIsRightHand && bLeftHandBusy);
}

void ABlackoutCharacter::UseItem(const FInputActionValue& InputActionValue)
{
	if (bIsThrowEnabled || bIsInventoryOpen) return;
	
	bool bIsRightHand = InputActionValue.Get<float>() > 0.f;

	if (!IsHandHoldingItem_Implementation(bIsRightHand)) return;

	if (bIsRightHand)
	{
		if (RightHandItem->Implements<UUsageInterface>() && IUsageInterface::Execute_CanBeUsed(RightHandItem, true))
			IUsageInterface::Execute_Use(RightHandItem);
	}
	else
	{
		if (LeftHandItem->Implements<UUsageInterface>() && IUsageInterface::Execute_CanBeUsed(LeftHandItem, false))
			IUsageInterface::Execute_Use(LeftHandItem);
	}
}

void ABlackoutCharacter::EnableThrow(const FInputActionValue& InputActionValue)
{
	bIsThrowEnabled = InputActionValue.Get<bool>();
}

void ABlackoutCharacter::ToggleInventory()
{
	ABlackoutHUD* BlackoutHUD = nullptr;
	ABlackoutPlayerController* PlayerController = Cast<ABlackoutPlayerController>(GetController());
	if (PlayerController)
	{
		BlackoutHUD = Cast<ABlackoutHUD>(PlayerController->GetHUD());
	}
	
	if (BlackoutHUD)
	{
		if (!bIsInventoryOpen)
		{
			BlackoutHUD->InitInventoryWidget(InventoryComponent);
			bIsInventoryOpen = true;
		}
		else
		{
			BlackoutHUD->CloseInventoryWidget();
			bIsInventoryOpen = false;
		}
	}
}
