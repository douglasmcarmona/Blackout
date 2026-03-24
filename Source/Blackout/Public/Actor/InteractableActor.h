#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "InteractableActor.generated.h"

// Stencil value used by the post-processing volume material to create the highlight effect
#define CUSTOM_DEPTH_HIGHLIGHT_STENCIL_VALUE 250.f;

/**
 * Represents actors which the player can interact with in some way through the Interact action.  
 */
UCLASS()
class BLACKOUT_API AInteractableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	AInteractableActor();
	// InteractionInterface override
	virtual void Highlight_Implementation() override;
	
	// InteractionInterface override
	virtual void Unhighlight_Implementation() override;
	
	// InteractionInterface override
	virtual void Interact_Implementation() override;
	
	// InteractionInterface override
	virtual bool IsPickable_Implementation() const override;
	
	// InteractionInterface override
	virtual bool IsStorable_Implementation() const override;
	
	// InteractionInterface override
	virtual bool IsThrowable_Implementation() const override;
	
	// InteractionInterface override
	virtual void Throw_Implementation(const FVector& ThrowForce) override;
	
	// InteractionInterface override
	virtual void PreparePickup_Implementation() override;
	
	// InteractionInterface override
	virtual UTexture2D* GetIcon_Implementation() override;

	/**
	 * When called, performs the required settings to become interactable again in the world, mainly after being thrown away
	 * by the player
	 */
	UFUNCTION(BlueprintCallable)
	void HandleDrop();
	
protected:
	/**
	 * Hints the player that they can interact with the actor, in case they're within the interaction range
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void ShowInteractWidget();

	/**
	 * Hides the interact widget. Executed when the player leaves the interaction range.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Highlight")
	void HideInteractWidget();

	virtual void ShowInteractWidget_Implementation() {}
	virtual void HideInteractWidget_Implementation() {}

	/**
	 * Visual representation of the InteractableActor in the world. It's set to be the RootComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/**
	 * Indicates if the InteractableActor can be picked up by the player
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPickable = true;

	/**
	 * Indicates if the InteractableActor can be stored in player's inventory
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsStorable = false;

	/**
	 * Indicates if the InteractableActor can be thrown away after being picked up/withdrawn
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsThrowable = false;

	/**
	 * Visual representation of the InteractableActor while inside the player's inventory
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> InventoryIcon;
};

