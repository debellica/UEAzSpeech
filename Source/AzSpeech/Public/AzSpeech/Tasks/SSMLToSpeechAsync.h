// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechSpeechSynthesisBase.h"
#include "SSMLToSpeechAsync.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API USSMLToSpeechAsync : public UAzSpeechSpeechSynthesisBase
{
	GENERATED_BODY()

public:
	/* Creates a SSML-To-Speech task that will convert your SSML file to speech */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Default", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SSML To Speech with Default Options"))
	static USSMLToSpeechAsync* SSMLToSpeech_DefaultOptions(UObject* WorldContextObject, const FString& SynthesisSSML);

	/* Creates a SSML-To-Speech task that will convert your SSML file to speech */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Custom", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SSML To Speech with Custom Options"))
	static USSMLToSpeechAsync* SSMLToSpeech_CustomOptions(UObject* WorldContextObject, const FString& SynthesisSSML, const FAzSpeechSettingsOptions& Options);
};