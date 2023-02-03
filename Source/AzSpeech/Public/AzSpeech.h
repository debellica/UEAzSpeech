// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include <Runtime/Launch/Resources/Version.h>

#if ENGINE_MAJOR_VERSION >= 5
#include <Modules/ModuleInterface.h>
#else
#include <Modules/ModuleManager.h>
#endif

/**
 *
 */

class FAzSpeechModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#ifdef AZSPEECH_RUNTIME_PLATFORM
private:
	void LoadRuntimeLibraries();
	void UnloadRuntimeLibraries();

	TArray<void*> RuntimeLibraries;
#endif
};
