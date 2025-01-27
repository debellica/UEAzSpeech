// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "AzSpeech/Structures/AzSpeechAudioInputDeviceInfo.h"
#include "AzSpeech/Structures/AzSpeechAnimationData.h"
#include "AzSpeech/Structures/AzSpeechVisemeData.h"
#include "AzSpeechHelper.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API UAzSpeechHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Helper function to qualify a Module Name string to a single string like /ModulePath/ */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", meta = (DisplayName = "Qualify Module Path"))
	static const FString QualifyModulePath(const FString& ModuleName);

	/* Helper function to qualify a path string to a single string like Full/File/Path/ */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", meta = (DisplayName = "Qualify Path"))
	static const FString QualifyPath(const FString& Path);

	/* Helper function to qualify the extension of a given file */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", meta = (DisplayName = "Qualify File Extension"))
	static const FString QualifyFileExtension(const FString& Path, const FString& Name, const FString& Extension);

	/* Helper function to qualify a WAV file path + name to a single string like Full/File/Path/Filename.wav */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", meta = (DisplayName = "Qualify WAV File Path"))
	static const FString QualifyWAVFileName(const FString& Path, const FString& Name)
	{
		return QualifyFileExtension(Path, Name, "wav");
	}

	/* Helper function to qualify a XML file path + name to a single string like Full/File/Path/Filename.xml */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", meta = (DisplayName = "Qualify XML File Path"))
	static const FString QualifyXMLFileName(const FString& Path, const FString& Name)
	{
		return QualifyFileExtension(Path, Name, "xml");
	}

	/* 
		Convert .wav file to USoundWave.

		[OutputModuleName, RelativeOutputDirectory, OutputAssetName]: Used to save the generated audio data in project's content. Set empty values to generate a transient Sound Wave

		OutputModuleName: Name of the module that will be used to save the generated audio data in project's content. Example: Game. 
		RelativeOutputDirectory: Directory where the sound wave will be saved
		OutputAssetName: Name of the generated Sound Wave

		Use GetAvailableContentModules or look at the Audio Generator tool to check available modules
	*/
	UFUNCTION(BlueprintCallable, Category = "AzSpeech", Meta = (DisplayName = "Convert .wav file to USoundWave"))
	static USoundWave* ConvertWavFileToSoundWave(const FString& FilePath, const FString& FileName, const FString& OutputModulePath = "", const FString& RelativeOutputDirectory = "", const FString& OutputAssetName = "");

	/* 
		Convert audio data (TArray<uint8>) to USoundWave.

		[OutputModuleName, RelativeOutputDirectory, OutputAssetName]: Used to save the generated audio data in project's content. Set empty values to generate a transient Sound Wave

		OutputModuleName: Name of the module that will be used to save the generated audio data in project's content. Example: Game. 
		RelativeOutputDirectory: Directory where the sound wave will be saved
		OutputAssetName: Name of the generated Sound Wave

		Use GetAvailableContentModules or look at the Audio Generator tool to check available modules
	*/
	UFUNCTION(BlueprintCallable, Category = "AzSpeech")
	static USoundWave* ConvertAudioDataToSoundWave(const TArray<uint8>& RawData, const FString& OutputModulePath = "", const FString& RelativeOutputDirectory = "", const FString& OutputAssetName = "");

	/* Load a given .xml file and return the content as string */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech", meta = (DisplayName = "Load XML to String"))
	static const FString LoadXMLToString(const FString& FilePath, const FString& FileName);

	/* Create a new directory in the specified location */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech")
	static const bool CreateNewDirectory(const FString& Path, const bool bCreateParents = true);

	/* Opens the desktop folder picker and return the selected folder path as string */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech")
	static const FString OpenDesktopFolderPicker();

	/* Check if the android platform already has permission and add if not */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech", meta = (DisplayName = "Check and Add Android Permission"))
	static const bool CheckAndroidPermission(const FString& InPermission);

	/* Check if the audio data is valid or not */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const bool IsAudioDataValid(const TArray<uint8>& RawData);

	/* Get the available audio input devices in the current platform */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech")
	static const TArray<FAzSpeechAudioInputDeviceInfo> GetAvailableAudioInputDevices();

	/* Get the audio input devices info by it's ID */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const FAzSpeechAudioInputDeviceInfo GetAudioInputDeviceInfoFromID(const FString& DeviceID);

	/* Check if the audio input device is currently available */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const bool IsAudioInputDeviceAvailable(const FString& DeviceID);

	/* Check if the device ID is valid */
	UFUNCTION(BlueprintPure, Category = "AzSpeech", Meta = (DisplayName = "Is Audio Input Device ID Valid"))
	static const bool IsAudioInputDeviceIDValid(const FString& DeviceID);

	/* Get available modules with content enabled */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const TArray<FString> GetAvailableContentModules();

	/* 
		Extract the Animation JSON property from Viseme Data.

		JSON Body Format:
		[
			FrameIndex: Integer,
			BlendShapes: [ 
				[ 
					Number, 
					... 
				], 
				[ 
					Number, 
					... 
				], 
				... 
			]
		]
	*/
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const FAzSpeechAnimationData ExtractAnimationDataFromVisemeData(const FAzSpeechVisemeData& VisemeData);
	
	/*
		Extract the Animation JSON property from Viseme Data Array.

		JSON Body Format:
		[
			FrameIndex: Integer,
			BlendShapes: [
				[
					Number,
					...
				],
				[
					Number,
					...
				],
				...
			]
		]
	*/
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static const TArray<FAzSpeechAnimationData> ExtractAnimationDataFromVisemeDataArray(const TArray<FAzSpeechVisemeData>& VisemeData);
	
	/*  */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static class UAzSpeechRecognizerTaskBase* CastToAzSpeechRecognizerTaskBase(UObject* Object);

	/*  */
	UFUNCTION(BlueprintPure, Category = "AzSpeech")
	static class UAzSpeechSynthesizerTaskBase* CastToAzSpeechSynthesizerTaskBase(UObject* Object);

	static const FString GetAzSpeechLogsBaseDir();
};
