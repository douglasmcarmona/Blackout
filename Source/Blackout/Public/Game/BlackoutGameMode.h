#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackoutGameMode.generated.h"

class UPaperNoteInfo;

/**
 * Out custom GameMode. Currently, it only stored the instance of the PaperNoteInfo data asset used to retrieve text
 * to be contained in those actors 
 */
UCLASS()
class BLACKOUT_API ABlackoutGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * The PaperNoteInfo data asset reference
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperNoteInfo> PaperNoteInformation;	
};
