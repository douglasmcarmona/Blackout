// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractableActor.h"

#include "Data/InventoryItemInfo.h"
#include "Game/BlackoutGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Util/BlackoutFunctionLibrary.h"

// Sets default values
AInteractableActor::AInteractableActor()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->CustomDepthStencilValue = CUSTOM_DEPTH_HIGHLIGHT_STENCIL_VALUE;
	HandleDrop();
}

void AInteractableActor::Highlight_Implementation()
{
	Mesh->SetRenderCustomDepth(true);
	ShowInteractWidget();
}

void AInteractableActor::Unhighlight_Implementation()
{
	Mesh->SetRenderCustomDepth(false);
	HideInteractWidget();
}

void AInteractableActor::Interact_Implementation()
{
	Unhighlight_Implementation();
}

bool AInteractableActor::IsPickable_Implementation() const
{
	return bIsPickable;
}

bool AInteractableActor::IsStorable_Implementation() const
{
	return bIsStorable;
}

bool AInteractableActor::IsThrowable_Implementation() const
{
	return bIsThrowable;
}

void AInteractableActor::Throw_Implementation(const FVector& ThrowForce)
{
	HandleDrop();
	Mesh->AddImpulse(ThrowForce);
}

void AInteractableActor::PreparePickup_Implementation()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UTexture2D* AInteractableActor::GetIcon_Implementation()
{	
	return UBlackoutFunctionLibrary::GetInventoryItemInfo(this)->GetInventoryItemByClass(GetClass())->ItemIcon;
}

FGuid AInteractableActor::GetPersistentGuid_Implementation()
{
	return PersistentGuid;
}

void AInteractableActor::SetPersistentGuid_Implementation(const FGuid& Guid)
{
	if (!PersistentGuid.IsValid())
	{
		PersistentGuid = Guid;		
	}
}

bool AInteractableActor::IsInOriginalState_Implementation() const
{
	return bIsInOriginalState;
}

void AInteractableActor::SetIsIsOriginalState_Implementation(const bool bInIsInOriginalState)
{
	bIsInOriginalState = bInIsInOriginalState;
}

#if WITH_EDITOR
void AInteractableActor::GenerateNewPersistentGuid()
{
	const FString& ObjectPath = GetActorGuid().ToString();
	PersistentGuid = FGuid::NewDeterministicGuid(ObjectPath);
}

void AInteractableActor::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);
	GenerateNewPersistentGuid();
}

void AInteractableActor::PostLoad()
{
	Super::PostLoad();
	GenerateNewPersistentGuid();
	bIsInOriginalState = true;
}
#endif

void AInteractableActor::Destroyed()
{
	Super::Destroyed();
	ABlackoutGameMode* BlackoutGameMode = Cast<ABlackoutGameMode>(UGameplayStatics::GetGameMode(this));
	if (BlackoutGameMode)
	{
		BlackoutGameMode->MarkActorAsPendingKill(GetWorld()->GetMapName(), PersistentGuid);
		bIsInOriginalState = false;
	}
}

void AInteractableActor::HandleDrop()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
