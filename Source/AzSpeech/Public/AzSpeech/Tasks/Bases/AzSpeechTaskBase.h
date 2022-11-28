// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

THIRD_PARTY_INCLUDES_START
#include <speechapi_cxx_audio_config.h>
THIRD_PARTY_INCLUDES_END

#include "AzSpeechTaskBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAzSpeechTaskGenericDelegate);

/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotPlaceable, Category = "AzSpeech")
class UAzSpeechTaskBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "AzSpeech", meta = (DisplayName = "Stop AzSpeech Task"))
	virtual void StopAzSpeechTask();

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	bool IsTaskActive() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	bool IsTaskReadyToDestroy() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const bool IsTaskStillValid(const UAzSpeechTaskBase* Test);

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const bool IsUsingAutoLanguage() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const FString GetTaskName() const;

	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	const FString GetLanguageID() const;

	virtual void SetReadyToDestroy() override;

protected:
	TSharedPtr<class FAzSpeechRunnableBase> RunnableTask;
	FName TaskName = NAME_None;
	
	FString LanguageId;
	const UObject* WorldContextObject;

	virtual bool StartAzureTaskWork();
	virtual void BroadcastFinalResult();

	mutable FCriticalSection Mutex;

#if WITH_EDITOR
	virtual void PrePIEEnded(bool bIsSimulating);

	bool bEndingPIE = false;
#endif

private:
	bool bIsTaskActive = false;
	bool bIsReadyToDestroy = false;
};