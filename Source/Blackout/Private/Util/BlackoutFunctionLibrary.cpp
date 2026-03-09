// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/BlackoutFunctionLibrary.h"

#include "Game/BlackoutGameMode.h"
#include "Kismet/GameplayStatics.h"

UPaperNoteInfo* UBlackoutFunctionLibrary::GetPaperNoteInfo(const UObject* WorldContextObject)
{
	if (ABlackoutGameMode* GameMode = Cast<ABlackoutGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameMode->PaperNoteInformation;
	}
	return nullptr;
}
