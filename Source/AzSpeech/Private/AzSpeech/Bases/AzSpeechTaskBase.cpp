// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech/Bases/AzSpeechTaskBase.h"
#include "AzSpeech/AzSpeechInternalFuncs.h"
#include "Misc/FileHelper.h"
#include "Async/Async.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

void UAzSpeechTaskBase::Activate()
{
	UE_LOG(LogAzSpeech, Display, TEXT("Task: %s (%s); Function: %s; Message: Activating task"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	AzSpeech::Internal::GetLanguageID(LanguageId);

	bIsTaskActive = true;
	bCanBroadcastFinal = true;

	Super::Activate();
	StartAzureTaskWork();

#if WITH_EDITOR
	FEditorDelegates::PrePIEEnded.AddUObject(this, &UAzSpeechTaskBase::PrePIEEnded);
#endif
}

void UAzSpeechTaskBase::StopAzSpeechTask()
{
	UE_LOG(LogAzSpeech, Display, TEXT("Task: %s (%s); Function: %s; Message: Finishing task"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));	
	bIsTaskActive = false;
}

bool UAzSpeechTaskBase::IsTaskActive() const
{
	return bIsTaskActive;
}

bool UAzSpeechTaskBase::IsTaskReadyToDestroy() const
{
	return bIsReadyToDestroy;
}

const bool UAzSpeechTaskBase::IsTaskStillValid(const UAzSpeechTaskBase* Test)
{
	bool bOutput = IsValid(Test) && Test->IsTaskActive() && !Test->IsTaskReadyToDestroy();

#if WITH_EDITOR
	bOutput = bOutput && !Test->bEndingPIE;
#endif

	return bOutput;
}

bool UAzSpeechTaskBase::StartAzureTaskWork()
{
	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Starting Azure SDK task"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	return AzSpeech::Internal::CheckAzSpeechSettings() && IsTaskStillValid(this);
}

void UAzSpeechTaskBase::SetReadyToDestroy()
{
	if (bIsReadyToDestroy)
	{
		return;
	}

	ClearBindings();
	UE_LOG(LogAzSpeech, Display, TEXT("Task: %s (%s); Function: %s; Message: Setting task as Ready to Destroy"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	bIsReadyToDestroy = true;
	bCanBroadcastFinal = false;

	Super::SetReadyToDestroy();
}

void UAzSpeechTaskBase::ConnectTaskSignals()
{
	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Connecting task signals"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
}

void UAzSpeechTaskBase::ClearBindings()
{
#if WITH_EDITOR
	if (FEditorDelegates::PrePIEEnded.IsBoundToObject(this))
	{
		FEditorDelegates::PrePIEEnded.RemoveAll(this);
	}
#endif
	
	if (!bAlreadyUnbound)
	{
		UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Removing existing bindings"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
	}

	bAlreadyUnbound = true;
}

void UAzSpeechTaskBase::BroadcastFinalResult()
{
	check(IsInGameThread());
	
	UE_LOG(LogAzSpeech, Display, TEXT("Task: %s (%s); Function: %s; Message: Task completed, broadcasting final result"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	bIsTaskActive = false;
	bCanBroadcastFinal = false;
}

const bool UAzSpeechTaskBase::IsUsingAutoLanguage() const
{
	return LanguageId.Equals("Auto", ESearchCase::IgnoreCase);
}

#if WITH_EDITOR
void UAzSpeechTaskBase::PrePIEEnded(bool bIsSimulating)
{
	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Trying to finish task due to PIE end"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
	
	bEndingPIE = true;
	StopAzSpeechTask();
}
#endif

void UAzSpeechTaskBase::ApplySDKSettings(const std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechConfig>& InSpeechConfig)
{
	if (!InSpeechConfig)
	{
		return;
	}

	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Applying Azure SDK Settings"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	EnableLogInConfiguration(InSpeechConfig);

	InSpeechConfig->SetProfanity(AzSpeech::Internal::GetProfanityFilter());

	if (IsUsingAutoLanguage())
	{
		UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Using auto language identification"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
		InSpeechConfig->SetProperty(Microsoft::CognitiveServices::Speech::PropertyId::SpeechServiceConnection_SingleLanguageIdPriority, "Latency");
	}
}

void UAzSpeechTaskBase::EnableLogInConfiguration(const std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechConfig>& InSpeechConfig)
{
	if (!InSpeechConfig)
	{
		return;
	}

	if (!AzSpeech::Internal::GetPluginSettings()->bEnableSDKLogs)
	{
		return;
	}
	
	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Enabling Azure SDK log"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));

	if (FString AzSpeechLogPath = AzSpeech::Internal::GetAzSpeechLogsBaseDir();
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*AzSpeechLogPath))
	{
		AzSpeechLogPath += "/UEAzSpeech " + FDateTime::Now().ToString() + ".log";

		if (FFileHelper::SaveStringToFile(FString(), *AzSpeechLogPath))
		{
			InSpeechConfig->SetProperty(Microsoft::CognitiveServices::Speech::PropertyId::Speech_LogFilename, TCHAR_TO_UTF8(*AzSpeechLogPath));
		}
	}
}

const FString UAzSpeechTaskBase::CancellationReasonToString(const Microsoft::CognitiveServices::Speech::CancellationReason& CancellationReason) const
{
	switch (CancellationReason)
	{
		case Microsoft::CognitiveServices::Speech::CancellationReason::Error:
			return FString("Error");

		case Microsoft::CognitiveServices::Speech::CancellationReason::EndOfStream:
			return FString("EndOfStream");

		case Microsoft::CognitiveServices::Speech::CancellationReason::CancelledByUser:
			return FString("CancelledByUser");

		default:
			return FString("Undefined");
	}
}

void UAzSpeechTaskBase::ProcessCancellationError(const Microsoft::CognitiveServices::Speech::CancellationErrorCode& ErrorCode, const std::string& ErrorDetails) const
{
	FString ErrorCodeStr;
	switch (ErrorCode)
	{
		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::NoError:
			ErrorCodeStr = "Error";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::AuthenticationFailure:
			ErrorCodeStr = "AuthenticationFailure";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::BadRequest:
			ErrorCodeStr = "BadRequest";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::TooManyRequests:
			ErrorCodeStr = "TooManyRequests";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::Forbidden:
			ErrorCodeStr = "Forbidden";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ConnectionFailure:
			ErrorCodeStr = "ConnectionFailure";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ServiceTimeout:
			ErrorCodeStr = "ServiceTimeout";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ServiceError:
			ErrorCodeStr = "ServiceError";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ServiceUnavailable:
			ErrorCodeStr = "ServiceUnavailable";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::RuntimeError:
			ErrorCodeStr = "RuntimeError";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ServiceRedirectTemporary:
			ErrorCodeStr = "ServiceRedirectTemporary";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::ServiceRedirectPermanent:
			ErrorCodeStr = "ServiceRedirectPermanent";
			break;

		case Microsoft::CognitiveServices::Speech::CancellationErrorCode::EmbeddedModelError:
			ErrorCodeStr = "EmbeddedModelError";
			break;

		default:
			ErrorCodeStr = "Undefined";
			break;
	}

	UE_LOG(LogAzSpeech_Internal, Error, TEXT("Task: %s (%s); Function: %s; Message: Error code: %s"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__), *ErrorCodeStr);

	const FString ErrorDetailsStr = UTF8_TO_TCHAR(ErrorDetails.c_str());
	UE_LOG(LogAzSpeech_Internal, Error, TEXT("Task: %s (%s); Function: %s; Message: Error Details: %s"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__), *ErrorDetailsStr);
	UE_LOG(LogAzSpeech_Internal, Error, TEXT("Task: %s (%s); Function: %s; Message: Log generated in directory: %s"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__), *AzSpeech::Internal::GetAzSpeechLogsBaseDir());
}

std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechConfig> UAzSpeechTaskBase::CreateSpeechConfig()
{
	UE_LOG(LogAzSpeech_Internal, Display, TEXT("Task: %s (%s); Function: %s; Message: Creating Azure SDK speech config"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
	
	const auto Settings = AzSpeech::Internal::GetAzSpeechKeys();
	const auto SpeechConfig = Microsoft::CognitiveServices::Speech::SpeechConfig::FromSubscription(Settings.at(0), Settings.at(1));

	if (!SpeechConfig)
	{
		UE_LOG(LogAzSpeech_Internal, Error, TEXT("Task: %s (%s); Function: %s; Message: Failed to create speech configuration"), *TaskName.ToString(), *FString::FromInt(GetUniqueID()), *FString(__func__));
		return nullptr;
	}

	return SpeechConfig;
}
