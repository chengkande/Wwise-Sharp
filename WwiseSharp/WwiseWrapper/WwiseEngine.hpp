//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#ifndef WWISEWRAPPER_WWISEENGINE_HPP
#define WWISEWRAPPER_WWISEENGINE_HPP

#include <stdlib.h> //for malloc & free
#include <windows.h> //for DWORD & others.
#include <exception>
#include <string>
#include <mbstring.h>

#include "SDK/include/AK/SoundEngine/Common/AkMemoryMgr.h"                  // Memory Manager
#include "SDK/include/AK/SoundEngine/Common/AkModule.h"                     // Default memory and stream managers
#include "SDK/include/AK/SoundEngine/Common/IAkStreamMgr.h"                 // Streaming Manager
#include "SDK/include/AK/Tools/Common/AkPlatformFuncs.h"                    // Thread defines
#include "SDK/samples/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h"								// Sample low-level I/O implementation (in $(WWISESDK)/samples/SoundEngine/Win32).
#include "SDK/include/AK/SoundEngine/Common/AkSoundEngine.h"                // Sound engine
#include "SDK/include/AK/MusicEngine/Common/AkMusicEngine.h"                // Music Engine
#include "SDK/include/AK/Plugin/AkRoomVerbFXFactory.h"						// Room Verb
#include "SDK/include/AK/Plugin/AkPeakLimiterFXFactory.h"					//Limiter
#ifdef WWISESHARP_VORBIS
	#include "SDK/include/AK/Plugin/AkVorbisFactory.h"						// Vorbis Plugin.
#endif
#ifdef DEBUG
	#include "SDk/include/AK/Comm/AkCommunication.h"						// Communication (is the component which allow Wwise editor to track Game Instance).
#endif

namespace WwiseWrapper
{
	class WwiseEngine
	{
		// We're using the default Low-Level I/O implementation that's part
		// of the SDK's sample code, with the file package extension.
		CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	public:
		bool Init();
		void ProcessAudio();
		void Term();

		void SetBasePath(const wchar_t* basePath);
		void SetLooseMediaBasePath(const wchar_t* basepath);
		void LoadBank(const wchar_t* bankName);
		void UnloadBank(const wchar_t* bankName);
		void PrepareBank(const wchar_t * bankName);
		const char* LoadEvent(const wchar_t ** in_ppszString, AkUInt32 in_uNumEvent);
		void ClearBanks();
		void UnloadPreparedEvent(const wchar_t ** in_ppszString, AkUInt32 in_uNumEvent);
		void ClearPreparedEvents();
		void RegisterGameObject(AkGameObjectID akId, const char* gameObjectLabel);
		void UnregisterGameObject(AkGameObjectID akId);
		void UnregisterAllGameObject();

		void PostEvent(const wchar_t* eventName, AkGameObjectID gameObject);
		void PostEvent(AkUniqueID eventId, AkGameObjectID gameObject);
		AkPlayingID PostMusicSyncEvent(const wchar_t* eventName, AkGameObjectID gameObject, AkUInt32 uFlags = 0, AkCallbackFunc callback = nullptr, void* pCookie = nullptr);
		//void MusicSyncCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

		void SetObjectRTPCValue(const wchar_t* rtpcName, AkRtpcValue value, AkGameObjectID  gameObjectId);
		void SetObjectRTPCValue(AkRtpcID rtpcId, AkRtpcValue value, AkGameObjectID  gameObjectId);

		void SetGlobalRTPCValue(const wchar_t* rtpcName, AkRtpcValue value);
		void SetGlobalRTPCValue(AkRtpcID rtpcId, AkRtpcValue value);

		void SetState(const wchar_t* stateGroup, const wchar_t* state);
		void SetState(AkStateGroupID stateGroupId, AkStateID stateId);

		void SetSwitch(const wchar_t* switchGroupName, const wchar_t* switchName, AkGameObjectID gameObjectId);
		void SetSwitch(AkStateGroupID switchGroupId, AkStateID switchId, AkGameObjectID gameObjectId);

		void SetListenerPosition(AkListenerPosition& akPosition);
		void SetListenerPosition(int listenerIndex, AkListenerPosition& akPosition);

		void SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position);
		void SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position, int listenerIndex);
		void SetMultiplePositions(AkGameObjectID gameObjectId, AkSoundPosition* akPositions, AkUInt16 numPositions);

		void SetActiveListeners(AkGameObjectID gameObjectId, AkUInt32 listenerMask);
		void SetPanningRule(AkPanningRule in_ePanningRule);
		void SetBusEffect(AkUniqueID in_audioNodeID, AkUInt32 in_uFXIndex, AkUniqueID in_shareSetID);
		void RegisterReverb();
		void GetSourcePlayPosition(AkPlayingID g_markersPlayingID, AkTimeMs* out_puPosition);
		AkSegmentInfo GetPlayingSegmentInfo(AkPlayingID in_playingID);
	};
}

#endif