﻿#include "pch.h"

/*
Based on Adrien Bock's original WwiseSharp
*/

#include "WwiseEngine.hpp"
#include <assert.h>
using namespace WwiseWrapper;

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().

	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}

#endif
}

bool WwiseEngine::Init()
{
	//
	// Create and initialize an instance of the default memory manager. Note
	// that you can override the default memory manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 32;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		throw std::exception("Could not create the memory manager.");
		return false;
	}

	//
	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		throw std::exception("Could not create the Streaming Manager");
		return false;
	}

	//
	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.        
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.

	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		throw std::exception("Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	//
	// Create the Sound Engine
	// Using default initialization parameters
	//

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	initSettings.uPrepareEventMemoryPoolID = AK::MemoryMgr::CreatePool(NULL, 16 * 1024 * 1024, 1024, AkMalloc);
	AK::MemoryMgr::SetPoolName(initSettings.uPrepareEventMemoryPoolID, L"PrepareEventPool");

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	//
	// Initialize the music engine
	// Using default initialization parameters
	//

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}
	/* -- Deprecated, probably
	#ifdef WWISESHARP_VORBIS
	AK::SoundEngine::RegisterCodec( AKCOMPANYID_AUDIOKINETIC,
	AKCODECID_VORBIS,
	CreateVorbisFilePlugin,
	CreateVorbisBankPlugin );
	#endif
	*/
#ifdef DEBUG
	//
	// Initialize communications (not in release build!)
	//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
#endif

	return false;
}

void WwiseEngine::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

void WwiseEngine::Term()
{
#ifdef DEBUG
	//
	// Terminate Communication Services
	//
	AK::Comm::Term();
#endif

	//
	// Terminate the music engine
	//

	AK::MusicEngine::Term();

	//
	// Terminate the sound engine
	//

	AK::SoundEngine::Term();

	// Terminate the streaming device and streaming manager

	// CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
}

void WwiseEngine::SetBasePath(const wchar_t* basePath)
{
	g_lowLevelIO.SetBasePath(basePath);
}

void WwiseEngine::SetLooseMediaBasePath(const wchar_t* basepath)
{
	g_lowLevelIO.SetAudioSrcPath(basepath);
}


//won't load media for banks with loose media. Use PrepareBank for that
void WwiseEngine::LoadBank(const wchar_t *bankName)
{
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.
	if (AK::SoundEngine::LoadBank(bankName, AK_DEFAULT_POOL_ID, bankID) != AK_Success)
		throw std::exception("Failed at loading soundbank.");
}

void WwiseEngine::UnloadBank(const wchar_t *bankName)
{
	if (AK::SoundEngine::UnloadBank(bankName, NULL) != AK_Success)
		throw std::exception("Failed at loading soundbank.");
}
//loads everything related to a bank, use for banks with loose media
void WwiseEngine::PrepareBank(const wchar_t * bankName)
{
	AK::SoundEngine::PrepareBank(AK::SoundEngine::PreparationType::Preparation_Load, bankName, AK::SoundEngine::AkBankContent::AkBankContent_All);
}
//takes array of strings, # of things in the array
const char* WwiseEngine::LoadEvent(const wchar_t ** in_ppszString, AkUInt32 in_uNumEvent)
{
	const char ** pEventNameArray = new const char *[in_uNumEvent];

	for (unsigned int x = 0; x < in_uNumEvent; x++)
	{
		size_t origsize = wcslen(in_ppszString[x]) + 1;
		size_t convertedChars = 0;
		const size_t newsize = origsize * 2;
		char* nstring = new char[newsize];
		wcstombs_s(&convertedChars, nstring, newsize, in_ppszString[x], _TRUNCATE);
		pEventNameArray[x] = nstring;
	}

	AK::SoundEngine::PrepareEvent(AK::SoundEngine::PreparationType::Preparation_Load, pEventNameArray, in_uNumEvent);
	return pEventNameArray[in_uNumEvent - 1]; //this is not returning correctly
}
void WwiseEngine::ClearBanks()
{
	AK::SoundEngine::ClearBanks();
}
void WwiseEngine::UnloadPreparedEvent(const wchar_t ** in_ppszString, AkUInt32 in_uNumEvent)
{
	const char ** pEventNameArray = new const char *[in_uNumEvent];

	for (unsigned int x = 0; x < in_uNumEvent; x++)
	{
		size_t origsize = wcslen(in_ppszString[x]) + 1;
		size_t convertedChars = 0;
		const size_t newsize = origsize * 2;
		char* nstring = new char[newsize];
		wcstombs_s(&convertedChars, nstring, newsize, in_ppszString[x], _TRUNCATE);
		pEventNameArray[x] = nstring;
	}

	AK::SoundEngine::PrepareEvent(AK::SoundEngine::PreparationType::Preparation_Unload, pEventNameArray, in_uNumEvent);
}
void WwiseEngine::ClearPreparedEvents()
{
	AK::SoundEngine::ClearPreparedEvents();
}

void WwiseEngine::RegisterGameObject(AkGameObjectID akId, const char* gameObjectLabel)
{
	AK::SoundEngine::RegisterGameObj(akId, gameObjectLabel);
}

void WwiseEngine::UnregisterGameObject(AkGameObjectID akId)
{
	AK::SoundEngine::UnregisterGameObj(akId);
}

void WwiseEngine::UnregisterAllGameObject()
{
	AK::SoundEngine::UnregisterAllGameObj();
}

void WwiseEngine::PostEvent(const wchar_t* eventName, AkGameObjectID gameObject)
{
	AK::SoundEngine::PostEvent(eventName, gameObject);
}
void WwiseEngine::PostEvent(AkUniqueID eventId, AkGameObjectID gameObject)
{
	AK::SoundEngine::PostEvent(eventId, gameObject);
}

AkPlayingID WwiseEngine::PostMusicSyncEvent(const wchar_t* eventName, AkGameObjectID gameObject, AkUInt32 uFlags, AkCallbackFunc callback, void* pCookie)
{
	return AK::SoundEngine::PostEvent(eventName, gameObject, uFlags, callback, pCookie);
}

void WwiseEngine::SetObjectRTPCValue(const wchar_t* rtpcName, AkRtpcValue value, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetRTPCValue(rtpcName, value, gameObjectId);
}
void WwiseEngine::SetObjectRTPCValue(AkRtpcID rtpcId, AkRtpcValue value, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetRTPCValue(rtpcId, value, gameObjectId);
}
void WwiseEngine::SetGlobalRTPCValue(const wchar_t* rtpcName, AkRtpcValue value)
{
	AK::SoundEngine::SetRTPCValue(rtpcName, value, AK_INVALID_GAME_OBJECT);
}
void WwiseEngine::SetGlobalRTPCValue(AkRtpcID rtpcId, AkRtpcValue value)
{
	AK::SoundEngine::SetRTPCValue(rtpcId, value, AK_INVALID_GAME_OBJECT);
}

void WwiseEngine::SetState(const wchar_t* stateGroup, const wchar_t* state)
{
	AK::SoundEngine::SetState(stateGroup, state);
}
void WwiseEngine::SetState(AkStateGroupID stateGroupId, AkStateID stateId)
{
	AK::SoundEngine::SetState(stateGroupId, stateId);
}

void WwiseEngine::SetSwitch(const wchar_t* switchGroupName, const wchar_t* switchName, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetSwitch(switchGroupName, switchName, gameObjectId);
}
/*
void WwiseEngine::SetSwitch(AkSwitchGroupID switchGroupId, AkSwitchStateID switchId, AkGameObjectID gameObjectId)
{
AK::SoundEngine::SetSwitch(switchGroupId, switchId, gameObjectId);
}
*/
void WwiseEngine::SetListenerPosition(AkListenerPosition& akPosition)
{
	AK::SoundEngine::SetListenerPosition(akPosition);
}
void WwiseEngine::SetListenerPosition(int listenerIndex, AkListenerPosition& akPosition)
{
	AK::SoundEngine::SetListenerPosition(akPosition, listenerIndex);
}

void WwiseEngine::SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position)
{
	AK::SoundEngine::SetPosition(gameObjectId, position);
}

void WwiseEngine::SetMultiplePositions(AkGameObjectID gameObjectId, AkSoundPosition* akPositions, AkUInt16 numPositions)
{
	AK::SoundEngine::SetMultiplePositions(gameObjectId, akPositions, numPositions, AK::SoundEngine::MultiPositionType_SingleSource);
}

void WwiseEngine::SetActiveListeners(AkGameObjectID gameObjectId, AkUInt32 listenerMask)
{
	AK::SoundEngine::SetActiveListeners(gameObjectId, listenerMask);
}

void WwiseEngine::SetPanningRule(AkPanningRule in_ePanningRule)
{
	AK::SoundEngine::SetPanningRule(in_ePanningRule, AkOutput_Main, (AkUInt32)0);
}

void WwiseEngine::SetBusEffect(AkUniqueID in_audioNodeID, AkUInt32 in_uFXIndex, AkUniqueID in_shareSetID)
{
	AK::SoundEngine::SetBusEffect(in_audioNodeID, in_uFXIndex, in_shareSetID);
}
/*	-- Deprecated fxn in 2016, now use includes only
void WwiseEngine::RegisterReverb()
{
AK::SoundEngine::RegisterPlugin(AkPluginTypeEffect,
AKCOMPANYID_AUDIOKINETIC,
AKEFFECTID_ROOMVERB,
CreateRoomVerbFX,
CreateRoomVerbFXParams);
AK::SoundEngine::RegisterPlugin(AkPluginTypeEffect,
AKCOMPANYID_AUDIOKINETIC,
AKEFFECTID_PEAKLIMITER,
CreatePeakLimiterFX,
CreatePeakLimiterFXParams);
AK::SoundEngine::RegisterPlugin(AkPluginTypeEffect,
AKCOMPANYID_AUDIOKINETIC,
AKEFFECTID_STEREODELAY,
CreateStereoDelayFX,
CreateStereoDelayFXParams);
}
*/
void WwiseEngine::GetSourcePlayPosition(AkPlayingID g_markersPlayingID, AkTimeMs* out_puPostition)
{
	AK::SoundEngine::GetSourcePlayPosition(g_markersPlayingID, out_puPostition);
}
AkSegmentInfo WwiseEngine::GetPlayingSegmentInfo(AkPlayingID in_playingID)
{
	AkSegmentInfo mSegmentInfo;
	AK::MusicEngine::GetPlayingSegmentInfo(in_playingID, mSegmentInfo);
	return mSegmentInfo;
}

void WwiseEngine::SetLanguage(const wchar_t* in_pLanguage)
{
	AK::StreamMgr::SetCurrentLanguage(in_pLanguage);
}

void WwiseEngine::SeekOnEvent(const wchar_t* in_pszEventName, AkGameObjectID in_gameObjectID, AkTimeMs in_iPosition)
{
	AK::SoundEngine::SeekOnEvent(
		in_pszEventName,								///< Name of the event
		in_gameObjectID,								///< Associated game object ID; use AK_INVALID_GAME_OBJECT to affect all game objects
		in_iPosition,									///< Desired position where playback should restart, expressed in a percentage of the file's total duration, between 0 and 1.f (see note above about infinite looping sounds)
		false,											///< If true, the final seeking position will be made equal to the nearest marker (see note above)	
		AK_INVALID_PLAYING_ID							///< Specify the playing ID for the seek to be applied to. Will result in the seek happening only on active actions of the playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any, to seek on all active actions of this event ID.	
	);
}
void WwiseEngine::SetAuxBus(AkGameObjectID gameObjectId, AkAuxSendValue* values, AkUInt16 numValues)
{
	AK::SoundEngine::SetGameObjectAuxSendValues(gameObjectId, values, numValues);
}