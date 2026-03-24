#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlackoutPlayerController.generated.h"

class UInputMappingContext;

/**
 * Our custom PlayerController implementation. Used mainly to store the default input mapping context reference
 */
UCLASS()
class BLACKOUT_API ABlackoutPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * Adds the InputMappingContext to the enhanced input subsystem
	 */
	virtual void BeginPlay() override;

	/**
	 * This game's input mapping context
	 */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> BlackoutPlayerContext;
};
