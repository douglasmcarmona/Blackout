// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PaperNoteInfo.h"

FPaperNote UPaperNoteInfo::FindPaperNoteById(int32 PaperNoteId)
{	
	for (FPaperNote& PaperNote : PaperNotes)
	{
		if (PaperNote.PaperNoteId == PaperNoteId)
		{
			return PaperNote;
		}
	}
	return FPaperNote();
}
