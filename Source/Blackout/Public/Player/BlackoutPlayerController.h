#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlackoutPlayerController.generated.h"

class UInputMappingContext;

/**
 * Used to select the mapping context that must be used at a given moment
 */
UENUM(BlueprintType)
enum class EMappingContext : uint8
{
	Default,
	PauseMenu
};

/**
 * Our custom PlayerController implementation. Used mainly to store the default input mapping context reference
 */
UCLASS()
class BLACKOUT_API ABlackoutPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * Sets the InputMappingContext this controller must use from this moment on
	 * @param SelectedMappingContext The value which represents the desired mapping context
	 */
	void ChangeMappingContext(const EMappingContext SelectedMappingContext) const;
	
protected:
	/**
	 * Adds the InputMappingContext to the enhanced input subsystem
	 */
	virtual void BeginPlay() override;

	/**
	 * This game's default input mapping context
	 */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> BlackoutPlayerContext;

	/**
	 * The input mapping context to be used when the game is paused
	 */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> PauseMenuContext;
};
