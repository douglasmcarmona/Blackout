// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "UObject/Object.h"
#include "Json.generated.h"

/**
 * Implements any required functionality that deals with json files
 */
UCLASS()
class BLACKOUT_API UJson : public UObject
{
	GENERATED_BODY()
	
public:	
	template<typename StructType>
	static void SaveUStructAsJson(const StructType& Struct, const FString& FilePath, const FString& FileName = FString("StructAsJson.json"))
	{
		FString Json;
		FJsonObjectConverter::UStructToJsonObjectString(Struct, Json);		
		TSharedPtr<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
		const FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath, FileName);
		FFileHelper::SaveStringToFile(Json, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
	}
	
	template <typename StructType>
	static void LoadJsonAsStruct(StructType& Struct, const FString& FilePath, const FString& FileName = FString("StructAsJson.json"))
	{
		const FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath, FileName);
		FString Json;
		FFileHelper::LoadFileToString(Json, *FullPath);
		FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Struct);
	}
};


