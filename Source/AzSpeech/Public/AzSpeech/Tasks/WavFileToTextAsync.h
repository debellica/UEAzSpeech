// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechRecognizerTaskBase.h"
#include "WavFileToTextAsync.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API UWavFileToTextAsync : public UAzSpeechRecognizerTaskBase
{
	GENERATED_BODY()

public:
	/* Creates a WavFile-To-Text task that will convert your Wav file to string */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Default", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = ".wav File To Text with Default Options"))
	static UWavFileToTextAsync* WavFileToText_DefaultOptions(UObject* WorldContextObject, const FString& FilePath, const FString& FileName, const FString& LanguageID = "Default", const FName PhraseListGroup = NAME_None);

	/* Creates a WavFile-To-Text task that will convert your Wav file to string */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Custom", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = ".wav File To Text with Custom Options"))
	static UWavFileToTextAsync* WavFileToText_CustomOptions(UObject* WorldContextObject, const FString& FilePath, const FString& FileName, const FAzSpeechSettingsOptions& Options, const FName PhraseListGroup = NAME_None);

	virtual void Activate() override;

protected:
	virtual bool StartAzureTaskWork() override;
	
private:
	FString FilePath;
	FString FileName;
};
