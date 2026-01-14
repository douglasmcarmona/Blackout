// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->CustomDepthStencilValue = CUSTOM_DEPTH_HIGHLIGHT_STENCIL_VALUE;
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
	return InteractionType >= EObjectInteraction::Pickable;
}

bool AInteractableActor::IsUsable_Implementation() const
{
	return InteractionType == EObjectInteraction::Usable;
}
