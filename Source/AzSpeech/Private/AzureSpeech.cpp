// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformProcess.h"

#define LOCTEXT_NAMESPACE "FAzSpeechModule"

void FAzSpeechModule::StartupModule()
{
#if PLATFORM_WINDOWS && defined _WIN64
	const FString PreDir = FPaths::Combine(*IPluginManager::Get().FindPlugin("AzSpeech")->GetBaseDir(),
	                                       TEXT("Source/ThirdParty/AzureWrapper/lib/"));

	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.core.dll", CoreDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.audio.sys.dll", AudioDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.kws.dll", KwsDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.lu.dll", LuDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.mas.dll", MasDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.silk_codec.dll", SilkDLL);
	LoadDependency(PreDir + "Microsoft.CognitiveServices.Speech.extension.codec.dll", CodecDLL);
#else
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("AzSpeechError", "Failed to load AzSpeech - Currently supports only Win64 builds"));
#endif
}

void FAzSpeechModule::ShutdownModule()
{
	FreeDependency(CoreDLL);
	FreeDependency(AudioDLL);
	FreeDependency(KwsDLL);
	FreeDependency(LuDLL);
	FreeDependency(MasDLL);
	FreeDependency(SilkDLL);
	FreeDependency(CodecDLL);
}

void FAzSpeechModule::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

void FAzSpeechModule::LoadDependency(const FString& Path, void*& Handle)
{
	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load required library %s. Plug-in will not be functional."), *Path);

		// const FText MsgInfo = ("AzSpeechError", FText::FromString(FString("Failed to load AzSpeech library: ") + Path));
		// FMessageDialog::Open(EAppMsgType::Ok, MsgInfo);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAzSpeechModule, AzSpeech)