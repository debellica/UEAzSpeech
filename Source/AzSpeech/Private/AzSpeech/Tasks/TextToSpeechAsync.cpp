// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech/Tasks/TextToSpeechAsync.h"
#include "AzSpeech/AzSpeechHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Async/Async.h"

UTextToSpeechAsync* UTextToSpeechAsync::TextToSpeech(const UObject* WorldContextObject, const FString& TextToConvert, const FString& VoiceName, const FString& LanguageId)
{
	UTextToSpeechAsync* const NewAsyncTask = NewObject<UTextToSpeechAsync>();
	NewAsyncTask->WorldContextObject = WorldContextObject;
	NewAsyncTask->SynthesisText = TextToConvert;
	NewAsyncTask->VoiceName = VoiceName;
	NewAsyncTask->LanguageId = LanguageId;
	NewAsyncTask->bIsSSMLBased = false;
	NewAsyncTask->TaskName = *FString(__func__);

	return NewAsyncTask;
}

void UTextToSpeechAsync::StopAzSpeechTask()
{
	Super::StopAzSpeechTask();

	if (AudioComponent.IsValid())
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
		AudioComponent.Reset();
	}
}

void UTextToSpeechAsync::BroadcastFinalResult()
{
	Super::BroadcastFinalResult();
}

void UTextToSpeechAsync::OnSynthesisUpdate()
{
	Super::OnSynthesisUpdate();

	if (!UAzSpeechTaskBase::IsTaskStillValid(this))
	{
		return;
	}

	if (CanBroadcastWithReason(LastSynthesisResult->Reason))
	{
		const TArray<uint8> LastBuffer = GetLastSynthesizedAudioData();
		if (!UAzSpeechHelper::IsAudioDataValid(LastBuffer))
		{
			return;
		}

		if (!UAzSpeechTaskBase::IsTaskStillValid(this))
		{
			return;
		}

		SynthesisCompleted.Broadcast(IsLastResultValid());

		// Clear bindings
		BroadcastFinalResult();

		AudioComponent = UGameplayStatics::CreateSound2D(WorldContextObject, UAzSpeechHelper::ConvertAudioDataToSoundWave(LastBuffer));
		AudioComponent->Play();
	}
}