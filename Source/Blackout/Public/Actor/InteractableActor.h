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
	
	// InteractionInterface override
	virtual FGuid GetPersistentGuid_Implementation() override;
	
	// InteractionInterface override
	virtual void SetPersistentGuid_Implementation(const FGuid& Guid) override;
	
	// InteractionInterface override
	virtual bool IsInOriginalState_Implementation() const override;
	
	// InteractionInterface override
	virtual void SetIsIsOriginalState_Implementation(const bool bInIsInOriginalState) override;

	// Object override
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	
	// Actor override
	virtual void PostLoad() override;
	
	// Actor override
	virtual void Destroyed() override;
	
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

	UFUNCTION(BlueprintCallable)
	void SetHighlightTarget(UPrimitiveComponent* NewHighlightTarget);
	
	/**
	 * Visual representation of the InteractableActor in the world. It's set to be the RootComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", Transient)
	TObjectPtr<UStaticMeshComponent> Mesh;

	/**
	 * Indicates if the InteractableActor can be picked up by the player
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bIsPickable = true;

	/**
	 * Indicates if the InteractableActor can be stored in player's inventory
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Interaction")
	bool bIsStorable = false;

	/**
	 * Indicates if the InteractableActor can be thrown away after being picked up/withdrawn
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bIsThrowable = false;

	/**
	 * A unique identifier used to track the actor's state. The guid is created once while in the editor (when the actor
	 * is placed in the level) and persists (hence the name) through inventory handling (store/withdraw) and level transitions.
	 */
	UPROPERTY(VisibleAnywhere, Category="InteractableActor", SaveGame)
	FGuid PersistentGuid;

	/**
	 * Tracks down whether an interactable actor is now at a different stage than that of the game start, such as to be
	 * stored in the inventory and withdrawn later on
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="InteractableActor")
	bool bIsInOriginalState = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="InteractableActor")
	TObjectPtr<UPrimitiveComponent> HighlightTarget;
	
private:
	/**
	 * Handy function to generate the actor's PersistentGuid value
	 */
	void GenerateNewPersistentGuid();
};

