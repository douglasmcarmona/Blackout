// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractableActor.h"

#include "Game/BlackoutGameInstance.h"

// Sets default values
AInteractableActor::AInteractableActor()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");	
	SetRootComponent(Mesh);
	Mesh->CustomDepthStencilValue = CUSTOM_DEPTH_HIGHLIGHT_STENCIL_VALUE;
	HandleDrop();
	PersistentGuid = FGuid::NewGuid();
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	if (BlackoutGameInstance->DoesMapHaveGuid(GetWorld()->GetMapName(), PersistentGuid))
	{
		Destroy();
	}
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
	return InventoryIcon;
}

FGuid AInteractableActor::GetGuid_Implementation()
{
	return PersistentGuid;
}

void AInteractableActor::HandleDrop()
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

