// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechSpeechSynthesisBase.h"
#include "TextToSpeechAsync.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API UTextToSpeechAsync : public UAzSpeechSpeechSynthesisBase
{
	GENERATED_BODY()

public:
	/* Creates a Text-To-Speech task that will convert your text to speech */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Default", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Text To Speech with Default Options"))
	static UTextToSpeechAsync* TextToSpeech_DefaultOptions(UObject* WorldContextObject, const FString& SynthesisText, const FString& VoiceName = "Default", const FString& LanguageID = "Default");

	/* Creates a Text-To-Speech task that will convert your text to speech */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Custom", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Text To Speech with Custom Options"))
	static UTextToSpeechAsync* TextToSpeech_CustomOptions(UObject* WorldContextObject, const FString& SynthesisText, const FAzSpeechSettingsOptions& Options);
};
