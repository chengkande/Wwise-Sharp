
#ifndef WWISESHARP_WWISESHARPGAMEOBJECT_HPP
#define WWISESHARP_WWISESHARPGAMEOBJECT_HPP

#include "WwiseEngine.hpp"

#include "WwiseSharpTransform.h"
#include "WwiseSharpMusicInfo.h"
#include "Wwise_IDs.h"

using namespace WwiseWrapper;
//typedef void(*AkCallbackFuncS)(AkCallbackType in_Type, AkCallbackInfo *in_pCallbackInfo);

#ifndef WWISE_UWP
namespace WwiseSharp
{
	public ref class WwiseSharpGameObject
	{
	protected:
		WwiseEngine* engine;

		AkTransform* thisPosition;
		AkVector *pos, *top, *front;

		AkGameObjectID objectId;
		char* label;
		bool hasHitMarker;
		//MusicSyncCallbackInfo = new AkCallbackInfo(null, objectId);
		//AkCallbackFunc MusicSyncCallbackFunc = new AkCallbackFunc(AK_MusicSyncUserCue, *MusicSyncCallbackInfo);

	public:
		AkTimeMs* out_puPosition;
		AkSegmentInfo* segInfo;
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
		void SetSinglePosition(WwiseSharpTransform^ position);
		void SetMultiplePositions(array<WwiseSharpTransform^>^ positions, unsigned short numPositions);
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource); //isMultiSource == add each source volume, false means that the object is multiple directions (refer to the documentation: Integration Details - 3D Positions)
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource, int listener);

		void SetObjectRTPCValue(unsigned long rtpcId, float value);
		void SetObjectRTPCValue(System::String^ rtpcName, float value);

		//void SetSwitch(unsigned long switchGroupId, unsigned long switchId);
		void SetSwitch(System::String^ switchGroupName, System::String^ switchName);

		void Seek(System::String^ eventName, unsigned long position); //Position is in Ms

		unsigned long^ GetSourcePlayPosition(unsigned long in_PlayingID);
		WwiseSharpMusicInfo^ GetPlayingSegmentInfo(unsigned long in_PlayingID);

		void SetTearBusSend1();
		void SetTearBusSend2();
		void RemoveTearReverbSend();
	};
}
/////////////////////////////////////////////////
/////////////////// U W P ///////////////////////
////////////////////////////////////////////////
#else
namespace WwiseSharp
{
	delegate void DCallbackFunc(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

	public ref class WwiseSharpGameObject sealed
	{
	private:
		AkTransform* thisPosition;
		AkVector *pos, *top, *front;
		WwiseEngine* engine;

		AkGameObjectID objectId;
		char* label;
		bool hasHitMarker;
		//MusicSyncCallbackInfo = new AkCallbackInfo(null, objectId);
		//AkCallbackFunc MusicSyncCallbackFunc = new AkCallbackFunc(AK_MusicSyncUserCue, *MusicSyncCallbackInfo);

		AkTimeMs* out_puPosition;
		AkSegmentInfo* segInfo;
		uint32 syncPlayingID;
		uint32 musicPlayingID, musicBeatDuration, musicBarDuration, musicGridDuration, musicGridOffset;

		//static DCallbackFunc^ pDCallbackFunc;
		//AkCallbackFunc mCallback;
		//System::Runtime::InteropServices::GCHandle gch;
		//void* thisPtr;
	internal:
		WwiseSharpGameObject(uint32 objectId, Platform::String^ label, WwiseEngine* engine);
	public:
		virtual ~WwiseSharpGameObject();

		AkGameObjectID GetAkObjectId();
		Platform::String^ GetLabelChar();
		//returns true if a marker has been hit since last checked
		void ActivateMarker();
		bool GetMarkerStatus();

		// This method take as parameter a BitMask to decide which listener should be used by this game object. By default, the game object only use Listener #1.
		// For more details, refer to the Wwise Documentation: WwiseSDK-Windows.chm > Sound Engine Integration Walkthrough > Integrate Wwise Elements into Your Game > Integrating Listeners > Assigning Game Objects to Listeners.
		// For exemple, if you want to only use Listener #2 only (most of the time you don't need more than 1 listener) for this game object, use MyGameObject.SetActiveListeners( (ulong)(2 << (ListenerIndex)) );
		void SetActiveListeners(uint32 listenerMask);

		[Windows::Foundation::Metadata::DefaultOverload]
		void PostEvent(Platform::String^ eventName);
		void PostEvent(uint32 eventId);
		//void MusicSyncCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
		//void StaticMusicSyncCallback(AkCallbackFunc callback, void *p);
		void PostMusicSyncEvent_Cue(Platform::String^ eventName);
		void PostMusicSyncEvent_Bar(Platform::String^ eventName);

		//Note: SinglePosition & MultiplePosition override each other between each ProcessAudio() call on WwiseSharpEngine object.
		void SetSinglePosition(WwiseSharpTransform^ position);
		void SetMultiplePositions(const Platform::Array<WwiseSharpTransform^>^ positions, uint16 numPositions);
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource); //isMultiSource == add each source volume, false means that the object is multiple directions (refer to the documentation: Integration Details - 3D Positions)
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource, int listener);
		[Windows::Foundation::Metadata::DefaultOverload]
		void SetObjectRTPCValue(Platform::String^ rtpcName, float32 value);
		void SetObjectRTPCValue(uint32 rtpcId, float32 value);


		//void SetSwitch(uint32 switchGroupId, uint32 switchId);
		void SetSwitch(Platform::String^ switchGroupName, Platform::String^ switchName);

		void Seek(Platform::String^ eventName, uint32 position); //Position is in Ms

		uint32 GetSourcePlayPosition(uint32 in_PlayingID);
		WwiseSharpMusicInfo GetPlayingSegmentInfo(uint32 in_PlayingID);

		void SetTearBusSend1();
		void SetTearBusSend2();
		void RemoveTearReverbSend();
	};
}


#endif
#endif