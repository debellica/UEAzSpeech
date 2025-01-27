// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechTaskBase.h"
#include "AzSpeech/Structures/AzSpeechVisemeData.h"
#include "AzSpeech/Structures/AzSpeechAnimationData.h"

THIRD_PARTY_INCLUDES_START
#include <speechapi_cxx_speech_synthesis_result.h>
THIRD_PARTY_INCLUDES_END

#include "AzSpeechSynthesizerTaskBase.generated.h"

class USoundWave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVisemeReceived, const FAzSpeechVisemeData, VisemeData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAudioDataSynthesisDelegate, const TArray<uint8>&, FinalAudioData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSoundWaveSynthesisDelegate, USoundWave*, GeneratedSound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBooleanSynthesisDelegate, const bool, bSuccess);

/**
 *
 */
UCLASS(Abstract, NotPlaceable, Category = "AzSpeech", meta = (ExposedAsyncProxy = AsyncTask))
class AZSPEECH_API UAzSpeechSynthesizerTaskBase : public UAzSpeechTaskBase
{
	GENERATED_BODY()

	friend class FAzSpeechSynthesisRunnable;

public:	
	/* Task delegate that will be called when dpdated */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzSpeechTaskGenericDelegate SynthesisUpdated;

	/* Task delegate that will be called when started */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzSpeechTaskGenericDelegate SynthesisStarted;

	/* Task delegate that will be called when failed */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzSpeechTaskGenericDelegate SynthesisFailed;

	/* Task delegate that will be called when receive a new viseme data */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FVisemeReceived VisemeReceived;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const FAzSpeechVisemeData GetLastVisemeData() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const TArray<FAzSpeechVisemeData> GetVisemeDataArray() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const TArray<uint8> GetAudioData() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const FAzSpeechAnimationData GetLastExtractedAnimationData() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const TArray<FAzSpeechAnimationData> GetExtractedAnimationDataArray() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const bool IsLastResultValid() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const FString GetSynthesisText() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech", Meta = (DisplayName = "Is SSML Based"))
	const bool IsSSMLBased() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const int32 GetConnectionLatency() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const int32 GetFinishLatency() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const int32 GetFirstByteLatency() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const int32 GetNetworkLatency() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const int32 GetServiceLatency() const;
	
protected:
	FString SynthesisText;
	
	void StartSynthesisWork(const std::shared_ptr<Microsoft::CognitiveServices::Speech::Audio::AudioConfig>& InAudioConfig);
	
	virtual void OnVisemeReceived(const FAzSpeechVisemeData& VisemeData);
	virtual void OnSynthesisUpdate(const std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesisResult>& LastResult);
	
private:
	std::vector<uint8_t> AudioData;
	TArray<FAzSpeechVisemeData> VisemeDataArray;
	bool bLastResultIsValid = false;

	int32 ConnectionLatency;
	int32 FinishLatency;
	int32 FirstByteLatency;
	int32 NetworkLatency;
	int32 ServiceLatency;
};
