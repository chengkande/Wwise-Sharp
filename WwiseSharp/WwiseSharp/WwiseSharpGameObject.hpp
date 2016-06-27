
#ifndef WWISESHARP_WWISESHARPGAMEOBJECT_HPP
#define WWISESHARP_WWISESHARPGAMEOBJECT_HPP

#include "WwiseEngine.hpp"

#include "WwiseSharpSoundPosition.hpp"
#include "WwiseSharpMusicInfo.h"

using namespace WwiseWrapper;
//typedef void(*AkCallbackFuncS)(AkCallbackType in_Type, AkCallbackInfo *in_pCallbackInfo);

namespace WwiseSharp
{
	public ref class WwiseSharpGameObject
	{
	protected:
		WwiseEngine* engine;

		AkGameObjectID objectId;
		char* label;
		bool hasHitMarker;
		bool tempMarkerStatus;
		//MusicSyncCallbackInfo = new AkCallbackInfo(null, objectId);
		//AkCallbackFunc MusicSyncCallbackFunc = new AkCallbackFunc(AK_MusicSyncUserCue, *MusicSyncCallbackInfo);

	public:
		AkTimeMs* out_puPosition;
		unsigned long syncPlayingID;
		unsigned long musicPlayingID, musicBeatDuration, musicBarDuration, musicGridDuration, musicGridOffset;

		delegate void DCallbackFunc(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
		static DCallbackFunc^ pDCallbackFunc;
		AkCallbackFunc mCallback;
		System::Runtime::InteropServices::GCHandle gch;
		void* thisPtr;
		WwiseSharpGameObject(unsigned int objectId, System::String^ label, WwiseEngine* engine);
		virtual ~WwiseSharpGameObject();

		AkGameObjectID GetAkObjectId();
		char* GetLabelChar();
		//returns true if a marker has been hit since last checked
		void ActivateMarker();
		bool GetMarkerStatus();

		// This method take as parameter a BitMask to decide which listener should be used by this game object. By default, the game object only use Listener #1.
		// For more details, refer to the Wwise Documentation: WwiseSDK-Windows.chm > Sound Engine Integration Walkthrough > Integrate Wwise Elements into Your Game > Integrating Listeners > Assigning Game Objects to Listeners.
		// For exemple, if you want to only use Listener #2 only (most of the time you don't need more than 1 listener) for this game object, use MyGameObject.SetActiveListeners( (ulong)(2 << (ListenerIndex)) );
		void SetActiveListeners(unsigned long listenerMask);

		void PostEvent(System::String^ eventName);
		void PostEvent(unsigned long eventId);
		void MusicSyncCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
		//void StaticMusicSyncCallback(AkCallbackFunc callback, void *p);
		void PostMusicSyncEvent_Cue(System::String^ eventName);
		void PostMusicSyncEvent_Bar(System::String^ eventName);

		//Note: SinglePosition & MultiplePosition override each other between each ProcessAudio() call on WwiseSharpEngine object.
		void SetSinglePosition(WwiseSharpSoundPosition^ position);
		void SetMultiplePositions(array<WwiseSharpSoundPosition^>^ positions, unsigned short numPositions);
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource); //isMultiSource == add each source volume, false means that the object is multiple directions (refer to the documentation: Integration Details - 3D Positions)
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource, int listener);

		void SetObjectRTPCValue(unsigned long rtpcId, float value);
		void SetObjectRTPCValue(System::String^ rtpcName, float value);

		void SetSwitch(unsigned long switchGroupId, unsigned long switchId);
		void SetSwitch(System::String^ switchGroupName, System::String^ switchName);

		unsigned long^ GetSourcePlayPosition(unsigned long in_PlayingID);
		WwiseSharpMusicInfo^ GetPlayingSegmentInfo(unsigned long in_PlayingID);
	};
}

#endif