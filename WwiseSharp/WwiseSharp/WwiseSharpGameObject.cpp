
#include "WwiseSharpGameObject.hpp"
using namespace WwiseSharp;

//#pragma unmanaged
//typedef void(__stdcall AkCallbackFunc)(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

delegate void DCallbackFunc(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

WwiseSharpGameObject::WwiseSharpGameObject(unsigned int wwiseObjectId, System::String^ wwiseLabel, WwiseEngine* engine)
{
	label = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(wwiseLabel).ToPointer());
	objectId = static_cast<AkGameObjectID>(wwiseObjectId);
	hasHitMarker = false;
	out_puPosition = 0;
	syncPlayingID = 0;
	musicPlayingID = 0;
	musicBarDuration = 0;
	musicBeatDuration = 0;
	musicGridDuration = 0;
	musicGridOffset = 0;

	pDCallbackFunc = gcnew DCallbackFunc(this, &WwiseSharpGameObject::MusicSyncCallback);
	gch = System::Runtime::InteropServices::GCHandle::Alloc(pDCallbackFunc);
	System::IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(pDCallbackFunc);
	mCallback = static_cast<AkCallbackFunc>(ip.ToPointer());
	thisPtr = System::Runtime::InteropServices::GCHandle::ToIntPtr(System::Runtime::InteropServices::GCHandle::Alloc(this)).ToPointer();
	this->engine = engine;
	
}

WwiseSharpGameObject::~WwiseSharpGameObject()
{
	System::Runtime::InteropServices::Marshal::FreeHGlobal(safe_cast<System::IntPtr>(label));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(safe_cast<System::IntPtr>(thisPtr));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(safe_cast<System::IntPtr>(mCallback));
	gch.Free();
}

AkGameObjectID WwiseSharpGameObject::GetAkObjectId()
{
	return objectId;
}
char* WwiseSharpGameObject::GetLabelChar()
{
	return label;
}
void WwiseSharpGameObject::ActivateMarker()
{
	hasHitMarker = true;
}
bool WwiseSharpGameObject::GetMarkerStatus()
{
	tempMarkerStatus = hasHitMarker;
	hasHitMarker = false;
	return tempMarkerStatus;
	//return hasHitMarker;
}

void WwiseSharpGameObject::SetActiveListeners(unsigned long listenerMask)
{
	engine->SetActiveListeners(objectId, listenerMask);
}

void WwiseSharpGameObject::PostEvent(System::String^ eventName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventName);
	engine->PostEvent(static_cast<LPCWSTR>(static_cast<void *>(p)), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
void WwiseSharpGameObject::PostEvent(unsigned long eventId)
{
	engine->PostEvent(static_cast<AkUniqueID>(eventId), objectId);
}
//may need to add callback info collection for bar/beat syncing of midi &/or conditionals for callback type
void WwiseSharpGameObject::MusicSyncCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	//void* p = in_pCallbackInfo->pCookie;
	//void* voidPtr = System::Runtime::InteropServices::GCHandle::ToIntPtr(System::Runtime::InteropServices::GCHandle::Alloc((System::IntPtr)p)).ToPointer();
	//System::Runtime::InteropServices::GCHandle h = System::Runtime::InteropServices::GCHandle::FromIntPtr((System::IntPtr)voidPtr);
	//WwiseSharpGameObject^ wGameObject = static_cast<WwiseSharpGameObject^>(h.Target);
	//wGameObject->ActivateMarker();
	this->ActivateMarker();

	AkMusicSyncCallbackInfo* in_info = static_cast<AkMusicSyncCallbackInfo*>(in_pCallbackInfo);
	musicPlayingID = static_cast<unsigned long>(in_info->playingID);
	segInfo = &in_info->segmentInfo;
	musicBarDuration = static_cast<unsigned long>(segInfo->fBarDuration * 1000);
	musicBeatDuration = static_cast<unsigned long>(segInfo->fBeatDuration * 1000);
	musicGridDuration = static_cast<unsigned long>(segInfo->fGridDuration * 1000);
	musicGridOffset = static_cast<unsigned long>(segInfo->fGridOffset * 1000);

	//h.Free();
}
//Wait for custom Cues to trigger callback
void WwiseSharpGameObject::PostMusicSyncEvent_Cue(System::String^ eventName)
{ 
	//void WwiseEngine::PostCallbackEvent(AkUniqueID eventId, AkGameObjectID gameObject, AkUInt32 uFlags = 0, AkCallbackFunc callback, void* pCookie)
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventName);
	syncPlayingID = static_cast<unsigned long>(engine->PostMusicSyncEvent(static_cast<LPCWSTR>(static_cast<void *>(p)), objectId, static_cast<AkUInt32>(0x202000), mCallback, thisPtr));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
//Trigger on every musical bar
void WwiseSharpGameObject::PostMusicSyncEvent_Bar(System::String^ eventName)
{
	//void WwiseEngine::PostCallbackEvent(AkUniqueID eventId, AkGameObjectID gameObject, AkUInt32 uFlags = 0, AkCallbackFunc callback, void* pCookie)
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventName);
	syncPlayingID = static_cast<unsigned long>(engine->PostMusicSyncEvent(static_cast<LPCWSTR>(static_cast<void *>(p)), objectId,  AK_EnableGetMusicPlayPosition));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpGameObject::SetSinglePosition(WwiseSharpTransform^ position)
{
	AkVector akVectorPosition;
	akVectorPosition.X = position->Xposition;
	akVectorPosition.Y = position->Yposition;
	akVectorPosition.Z = position->Zposition;
	
	AkVector akVectorOrientation = { 0.0, 1.0, 0.0 };

	AkVector top = {0.0, 1.0, 0.0};

	AkTransform akSoundPosition;
	akSoundPosition.SetPosition(akVectorPosition);
	akSoundPosition.SetOrientation(akVectorOrientation, top);

	engine->SetPosition(objectId, akSoundPosition);
}
void WwiseSharpGameObject::SetMultiplePositions(array<WwiseSharpTransform^>^ positions, unsigned short numPositions)
{
	AkTransform* akSoundPositions = new AkTransform[numPositions];
	//Fucking ignore orientation completely and normalize it so the profiler shuts up
	AkVector akVectorOrientation = { 0.0, 1.0, 0.0 };
	AkVector top = { 0.0, 1.0, 0.0 };
	for (unsigned short x = 0; x < numPositions; x++)
	{
		AkVector akVectorPosition;
		akVectorPosition.X = positions[x]->Xposition;
		akVectorPosition.Y = positions[x]->Yposition;
		akVectorPosition.Z = positions[x]->Zposition;

		akSoundPositions[x].SetPosition(akVectorPosition);
		akSoundPositions[x].SetOrientation(akVectorOrientation, top);
	}

	engine->SetMultiplePositions(objectId, akSoundPositions, static_cast<AkUInt16>(numPositions));
}

//Update RTPC value for individual objects - doesn't work on Busses, will break SetGameParameter events
void WwiseSharpGameObject::SetObjectRTPCValue(unsigned long rtpcId, float value)
{
	engine->SetObjectRTPCValue(static_cast<AkRtpcID>(rtpcId), static_cast<AkRtpcValue>(value), objectId);
}
void WwiseSharpGameObject::SetObjectRTPCValue(System::String^ rtpcName, float value)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(rtpcName);
	engine->SetObjectRTPCValue(static_cast<LPCWSTR>(static_cast<void *>(p)), static_cast<AkRtpcValue>(value), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpGameObject::SetSwitch(unsigned long switchGroupId, unsigned long switchId)
{
	engine->SetSwitch(static_cast<AkSwitchGroupID>(switchGroupId), static_cast<AkSwitchStateID>(switchId), objectId);
}
void WwiseSharpGameObject::SetSwitch(System::String^ switchGroupName, System::String^ switchName)
{
	System::IntPtr p1 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(switchGroupName);
	System::IntPtr p2 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(switchName);
	engine->SetSwitch(static_cast<LPCWSTR>(static_cast<void *>(p1)), static_cast<LPCWSTR>(static_cast<void *>(p2)), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p1);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p2);
}

void WwiseSharpGameObject::Seek(System::String^ eventName, unsigned long position)
{
	System::IntPtr p1 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventName);
	engine->SeekOnEvent(static_cast<LPCWSTR>(static_cast<void *>(p1)), objectId, position);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p1);
}

unsigned long^ WwiseSharpGameObject::GetSourcePlayPosition(unsigned long in_PlayingID)
{
	engine->GetSourcePlayPosition(static_cast<AkPlayingID>(in_PlayingID), out_puPosition);

	void* pu = out_puPosition; 
	void* voidPtr = System::Runtime::InteropServices::GCHandle::ToIntPtr(System::Runtime::InteropServices::GCHandle::Alloc((System::IntPtr)pu)).ToPointer();
	System::Runtime::InteropServices::GCHandle h = System::Runtime::InteropServices::GCHandle::FromIntPtr((System::IntPtr)voidPtr);
	unsigned long^ playingPosition = static_cast<unsigned long^>(h.Target);

	return playingPosition;
}

WwiseSharpMusicInfo^ WwiseSharpGameObject::GetPlayingSegmentInfo(unsigned long in_PlayingID)
{
	AkSegmentInfo mSegmentInfo;
	WwiseSharpMusicInfo^ musicInfo = gcnew WwiseSharpMusicInfo;

	//null check

	mSegmentInfo = engine->GetPlayingSegmentInfo(static_cast<AkPlayingID>(in_PlayingID));

	if (mSegmentInfo.iCurrentPosition != NULL)
	{
		musicInfo->currentPosition = static_cast<unsigned long>(mSegmentInfo.iCurrentPosition);
		musicInfo->preEntryDuration = static_cast<unsigned long>(mSegmentInfo.iPreEntryDuration);
		musicInfo->activeDuration = static_cast<unsigned long>(mSegmentInfo.iActiveDuration);
		musicInfo->postExitDuration = static_cast<unsigned long>(mSegmentInfo.iPostExitDuration);
		musicInfo->remainingLookAheadTime = static_cast<unsigned long>(mSegmentInfo.iRemainingLookAheadTime);

		musicInfo->playingID = musicPlayingID;
		musicInfo->beatDuration = musicBeatDuration;
		musicInfo->barDuration = musicBarDuration;
		musicInfo->gridDuration = musicGridDuration;
		musicInfo->gridOffset = musicGridOffset;
	}
	
	return musicInfo;
}
void WwiseSharpGameObject::SetTearBusSend1()
{
	AkAuxSendValue value;
	value.auxBusID = AK::AUX_BUSSES::TEAR_MUSIC_1;
	value.fControlValue = 1.0f;
	engine->SetAuxBus(objectId, &value, 1);
}
void WwiseSharpGameObject::SetTearBusSend2()
{
	AkAuxSendValue value;
	value.auxBusID = AK::AUX_BUSSES::TEAR_MUSIC_2;
	value.fControlValue = 1.0f;
	engine->SetAuxBus(objectId, &value, 1);
}
void WwiseSharpGameObject::RemoveTearReverbSend()
{
	engine->SetAuxBus(objectId, NULL, 0);
}