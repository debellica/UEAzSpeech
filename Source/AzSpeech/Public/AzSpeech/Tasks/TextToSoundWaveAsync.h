// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechAudioDataSynthesisBase.h"
#include "TextToSoundWaveAsync.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API UTextToSoundWaveAsync : public UAzSpeechAudioDataSynthesisBase
{
	GENERATED_BODY()

public:
	/* Task delegate that will be called when completed */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FSoundWaveSynthesisDelegate SynthesisCompleted;

	/* Creates a Text-To-SoundWave task that will convert your text to a USoundWave */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Default", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Text To Sound Wave with Default Options"))
	static UTextToSoundWaveAsync* TextToSoundWave_DefaultOptions(UObject* WorldContextObject, const FString& SynthesisText, const FString& VoiceName = "Default", const FString& LanguageID = "Default");

	/* Creates a Text-To-SoundWave task that will convert your text to a USoundWave */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech | Custom", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Text To Sound Wave with Custom Options"))
	static UTextToSoundWaveAsync* TextToSoundWave_CustomOptions(UObject* WorldContextObject, const FString& SynthesisText, const FAzSpeechSettingsOptions& Options);

protected:
	virtual void BroadcastFinalResult() override;
};
