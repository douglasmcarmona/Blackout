#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperNoteInfo.generated.h"

/**
 * Simple id-value struct used to handle paper notes data in the game
 */
USTRUCT(BlueprintType)
struct FPaperNote
{
	GENERATED_BODY()

	/**
	 * Used to identify the paper note element in the array
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PaperNoteId;

	/**
	 * The paper note content
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PaperNoteText;
	
	FPaperNote()
	{
		PaperNoteId = -1;
		PaperNoteText = FText::GetEmpty();
	}
};

/**
 * Represents a data collection of all possible values the paper notes can have. Once a paper note actor receives an id,
 * its content will be filled with the corresponding text from this collection.
 */
UCLASS()
class BLACKOUT_API UPaperNoteInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Gets the paper note data for the given id
	 * @param PaperNoteId The id of the text content
	 * @return The paper note element or the default one, if it doesn't exist  
	 */
	UFUNCTION(BlueprintPure)
	FPaperNote FindPaperNoteById(int32 PaperNoteId);

	/**
	 * Gathers all paper note data in the game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPaperNote> PaperNotes;	
};
