// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech/Tasks/TextToSoundWaveAsync.h"
#include "AzSpeech/AzSpeechHelper.h"
#include <Sound/SoundWave.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(TextToSoundWaveAsync)
#endif

UTextToSoundWaveAsync* UTextToSoundWaveAsync::TextToSoundWave(UObject* WorldContextObject, const FString& SynthesisText, const FString& VoiceName, const FString& LanguageID)
{
	UTextToSoundWaveAsync* const NewAsyncTask = NewObject<UTextToSoundWaveAsync>();
	NewAsyncTask->WorldContextObject = WorldContextObject;
	NewAsyncTask->SynthesisText = SynthesisText;
	NewAsyncTask->VoiceName = VoiceName;
	NewAsyncTask->LanguageID = LanguageID;
	NewAsyncTask->bIsSSMLBased = false;
	NewAsyncTask->TaskName = *FString(__func__);
	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}

void UTextToSoundWaveAsync::BroadcastFinalResult()
{
	Super::BroadcastFinalResult();

	FScopeLock Lock(&Mutex);

	if (SynthesisCompleted.IsBound())
	{
		const TArray<uint8> LastBuffer = GetAudioData();
		SynthesisCompleted.Broadcast(UAzSpeechHelper::ConvertAudioDataToSoundWave(LastBuffer));
		SynthesisCompleted.Clear();
	}
}

void UTextToSoundWaveAsync::OnSynthesisUpdate(const std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesisResult>& LastResult)
{
	Super::OnSynthesisUpdate(LastResult);

	if (!UAzSpeechTaskBase::IsTaskStillValid(this))
	{
		return;
	}

	if (CanBroadcastWithReason(LastResult->Reason))
	{
		FScopeLock Lock(&Mutex);

		BroadcastFinalResult();
	}
}