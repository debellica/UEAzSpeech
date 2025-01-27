// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech/Tasks/SSMLToWavFileAsync.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(SSMLToWavFileAsync)
#endif

USSMLToWavFileAsync* USSMLToWavFileAsync::SSMLToWavFile_DefaultOptions(UObject* WorldContextObject, const FString& SynthesisSSML, const FString& FilePath, const FString& FileName)
{
	return SSMLToWavFile_CustomOptions(WorldContextObject, SynthesisSSML, FilePath, FileName, FAzSpeechSettingsOptions());
}

USSMLToWavFileAsync* USSMLToWavFileAsync::SSMLToWavFile_CustomOptions(UObject* WorldContextObject, const FString& SynthesisSSML, const FString& FilePath, const FString& FileName, const FAzSpeechSettingsOptions& Options)
{
	USSMLToWavFileAsync* const NewAsyncTask = NewObject<USSMLToWavFileAsync>();
	NewAsyncTask->WorldContextObject = WorldContextObject;
	NewAsyncTask->SynthesisText = SynthesisSSML;
	NewAsyncTask->TaskOptions = GetValidatedOptions(Options);
	NewAsyncTask->FilePath = FilePath;
	NewAsyncTask->FileName = FileName;
	NewAsyncTask->bIsSSMLBased = true;
	NewAsyncTask->TaskName = *FString(__func__);
	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}