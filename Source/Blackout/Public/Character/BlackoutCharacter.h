#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interaction/HandInterface.h"
#include "BlackoutCharacter.generated.h"

class UInventoryComponent;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UBillboardComponent;

/**
 * Our player in the game. A simple first person character. Capable of moving, looking around, interacting with InteractableActors,
 * and using, throwing away and storing items
 */
UCLASS()
class BLACKOUT_API ABlackoutCharacter : public ACharacter, public IHandInterface
{
	GENERATED_BODY()

public:	
	ABlackoutCharacter();

	/**
	 * Performs a sphere trace with InteractionRadius and MaxInteraction values set to sweep the world looking for 
	 * InteractableActors. Upon a blocking hit, the InteractableActors are told to highlight themselves to indicate
	 * that an interaction is possible
	 */
	virtual void Tick(float DeltaSeconds) override;
	
	// ACharacter override
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// HandInterface override
	virtual AActor* GetRightHandItem_Implementation() const override;
	
	// HandInterface override
	virtual AActor* GetLeftHandItem_Implementation() const override;
	
	// HandInterface override
	virtual void SetRightHandItem_Implementation(AActor* Item) override;
	
	// HandInterface override
	virtual void SetLeftHandItem_Implementation(AActor* Item) override;
	
	// HandInterface override
	virtual bool IsHandHoldingItem_Implementation(const bool bIsRightHand) const override;
	
	// HandInterface override
	virtual FVector GetHandLocation_Implementation(const bool bIsRightHand) const override;
	
	// HandInterface override
	virtual AActor* DropLeftHandItem_Implementation() override;
	
	// HandInterface override
	virtual AActor* DropRightHandItem_Implementation() override;

	/**
	 * Indicates if the player is browsing the inventory at the moment. All other actions are blocked if that's the case
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsInventoryOpen = false;

	/**
	 * Indicates if the player is reading a paper note. All other actions but "toggle inventory" are blocked
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsReadingNote = false;

protected:
	/**
	 * - Validates InputAction properties
	 * - Binds a lambda function to the delegate broadcast by InventoryComponent to destroy the stored item 
	 */
	virtual void BeginPlay() override;

	/**
	 * Player camera. Simulates their point of view
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;

	/**
	 * Simulates the player's right hand. Pickable interactable actors are attached to this scene component, since the
	 * player doesn't have a mesh
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<USceneComponent> RightHand;

	/**
	 * Simulates the player's left hand. Pickable interactable actors are attached to this scene component, since the
	 * player doesn't have a mesh
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<USceneComponent> LeftHand;

	/**
	 * InputAction that handles player movement
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	/**
	 * InputAction that handles player vision
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAroundAction;

	/**
	 * InputAction that handles player's interaction with InteractableActor instances
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;

	/**
	 * InputAction that handles usage of items currently in either hand
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> UseItemAction;

	/**
	 * When triggered, this action overrides the use item inputs, so that items are thrown instead
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> EnableThrowAction;

	/**
	 * Toggles player's inventory. While it's active, all other actions are blocked
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ToggleInventoryAction;

	/**
	 * Indicates if player's look up and look down actions are inverted, given the input values
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bInvertY = false;

	/**
	 * Simulates the position of the player's eyes
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float EyesightZ = 50.f;

	/**
	 * An InteractableActor can be interacted with if the player is less or equal than this distance away from it 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float MaxInteractableDistance = 200.f;

	/**
	 * Gives a tolerance radius from the location the player is looking at to enable interaction
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius = 20.f;

	/**
	 * Keeps track of the actor hit by the sphere trace at the current frame. If it's different from LastActor, and it's an
	 * InteractableActor, it should highlight itself
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> ThisActor;

	/**
	 * Keeps track of the actor hit by the sphere trace at the current frame. If it's different from ThisActor, and it's an
	 * InteractableActor, it should unhighlight itself
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> LastActor;

	/**
	 * Stores a reference to the item currently attached to the player's RightHand
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> RightHandItem;

	/**
	 * Stores a reference to the item currently attached to the player's RightHand
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> LeftHandItem;

	/**
	 * Enables the player to use an inventory to store any storable InteractableActor 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	

private:
	/**
	 * Move InputAction callback. Adds movement input to the player controller
	 * @param InputValue The current value of Move input, processed as a FVector2D
	 */
	void Move(const FInputActionValue& InputValue);

	/**
	 * LookAround InputAction callback. Adds yaw and pitch inputs to the player controller
	 * @param InputValue The current value of LookAround input, processed as a FVector2D
	 */
	void LookAround(const FInputActionValue& InputValue);

	/**
	 * Interact InputAction callback
	 */
	void Interact();

	/**
	 * Checks if the input actor is an InteractableActor
	 * @param Actor The actor to have its condition checked
	 * @return True if the actor is an InteractableActor. False otherwise
	 */
	bool IsInteractableActor(const AActor* Actor) const;

	/**
	 * Gets which hand a picked up InteractableActor should be placed into 
	 * @return 0 or 2 if the right hand is free
	 * @return 1 if the left hand is free
	 * @return 3 if both hands are busy
	 */
	uint32 GetFreeHand() const;

	/**
	 * Attempt to use the item in a hand
	 * @param InputActionValue The value of the Use input, processed as a boolean (true indicates the right hand and false
	 * indicates the left hand)
	 */
	void UseItem(const FInputActionValue& InputActionValue);

	/**
	 * EnableThrow InputAction callback. Works as a switch that controls that mechanic
	 * @param InputActionValue The value of the EnableAction input, processed as a boolean (sets the value of bIsThrowEnabled property)
	 */
	void EnableThrow(const FInputActionValue& InputActionValue);

	/**
	 * ToggleInventory InputAction callback. Shows/hides the inventory to the player
	 */
	void ToggleInventory();

	/**
	 * Callback binded to the OnDestroyed delegate of RightHandItem and LeftHandItem properties. Properly reset the pointers 
	 * @param DestroyedItem The item that was destroyed
	 */
	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedItem);

	/**
	 * Enables and disables the ThrowItem action
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsThrowEnabled = false;
};
