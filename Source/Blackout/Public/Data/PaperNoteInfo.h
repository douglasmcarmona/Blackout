// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperNoteInfo.generated.h"

USTRUCT(BlueprintType)
struct FPaperNote
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PaperNoteId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PaperNoteText;
	
	FPaperNote()
	{
		PaperNoteId = -1;
		PaperNoteText = FText::GetEmpty();
	}
};

UCLASS()
class BLACKOUT_API UPaperNoteInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintPure)
	FPaperNote FindPaperNoteById(int32 PaperNoteId);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPaperNote> PaperNotes;	
};
