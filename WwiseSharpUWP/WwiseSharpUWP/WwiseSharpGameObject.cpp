#include "pch.h"
#include "WwiseSharpGameObject.hpp"
using namespace WwiseSharp;

//#pragma unmanaged
//typedef void(__stdcall AkCallbackFunc)(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

//public delegate void DCallbackFunc(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

WwiseSharpGameObject::WwiseSharpGameObject(uint32 wwiseObjectId, Platform::String^ wwiseLabel, WwiseEngine* engine)
{
	const wchar_t* w = wwiseLabel->Data();
	size_t i;
	char *c = new char[wwiseLabel->Length() + 1];
	memset(c, 0, wwiseLabel->Length() + 1);
	wcstombs_s(&i, c, (size_t)wwiseLabel->Length() + 1, wwiseLabel->Data(), (size_t)wwiseLabel->Length() + 1);
	label = c;

	objectId = static_cast<AkGameObjectID>(wwiseObjectId);
	hasHitMarker = false;
	out_puPosition = 0;
	syncPlayingID = 0;
	musicPlayingID = 0;
	musicBarDuration = 0;
	musicBeatDuration = 0;
	musicGridDuration = 0;
	musicGridOffset = 0;

	//pDCallbackFunc = gcnew DCallbackFunc(this, &WwiseSharpGameObject::MusicSyncCallback);
	//gch = System::Runtime::InteropServices::GCHandle::Alloc(pDCallbackFunc);
	//System::IntPtr ip = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(pDCallbackFunc);
	//mCallback = static_cast<AkCallbackFunc>(ip.ToPointer());
	//thisPtr = System::Runtime::InteropServices::GCHandle::ToIntPtr(System::Runtime::InteropServices::GCHandle::Alloc(this)).ToPointer();
	this->engine = engine;
	
}

WwiseSharpGameObject::~WwiseSharpGameObject()
{
	
}

AkGameObjectID WwiseSharpGameObject::GetAkObjectId()
{
	return objectId;
}
Platform::String^ WwiseSharpGameObject::GetLabelChar()
{
	std::string s_str = std::string(label);
	std::wstring w_str = std::wstring(s_str.begin(), s_str.end());
	const wchar_t* w_char = w_str.c_str();
	return ref new Platform::String(w_char);
}
void WwiseSharpGameObject::ActivateMarker()
{
	hasHitMarker = true;
}
bool WwiseSharpGameObject::GetMarkerStatus()
{
	bool tempMarkerStatus = hasHitMarker;
	hasHitMarker = false;
	return tempMarkerStatus;
	//return hasHitMarker;
}

void WwiseSharpGameObject::SetActiveListeners(uint32 listenerMask)
{
	engine->SetActiveListeners(objectId, listenerMask);
}

void WwiseSharpGameObject::PostEvent(Platform::String^ eventName)
{
	engine->PostEvent(eventName->Data(), objectId);
}
void WwiseSharpGameObject::PostEvent(uint32 eventId)
{
	engine->PostEvent(static_cast<AkUniqueID>(eventId), objectId);
}
//may need to add callback info collection for bar/beat syncing of midi &/or conditionals for callback type
/*
void WwiseSharpGameObject::MusicSyncCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	//void* p = in_pCallbackInfo->pCookie;
	//void* voidPtr = System::Runtime::InteropServices::GCHandle::ToIntPtr(System::Runtime::InteropServices::GCHandle::Alloc((System::IntPtr)p)).ToPointer();
	//System::Runtime::InteropServices::GCHandle h = System::Runtime::InteropServices::GCHandle::FromIntPtr((System::IntPtr)voidPtr);
	//WwiseSharpGameObject^ wGameObject = static_cast<WwiseSharpGameObject^>(h.Target);
	//wGameObject->ActivateMarker();
	this->ActivateMarker();

	AkMusicSyncCallbackInfo* in_info = static_cast<AkMusicSyncCallbackInfo*>(in_pCallbackInfo);
	musicPlayingID = static_cast<uint32>(in_info->playingID);
	segInfo = &in_info->segmentInfo;
	musicBarDuration = static_cast<uint32>(segInfo->fBarDuration * 1000);
	musicBeatDuration = static_cast<uint32>(segInfo->fBeatDuration * 1000);
	musicGridDuration = static_cast<uint32>(segInfo->fGridDuration * 1000);
	musicGridOffset = static_cast<uint32>(segInfo->fGridOffset * 1000);

	//h.Free();
}
*/
//Wait for custom Cues to trigger callback
void WwiseSharpGameObject::PostMusicSyncEvent_Cue(Platform::String^ eventName)
{ 
	//void WwiseEngine::PostCallbackEvent(AkUniqueID eventId, AkGameObjectID gameObject, AkUInt32 uFlags = 0, AkCallbackFunc callback, void* pCookie)
	syncPlayingID = static_cast<uint32>(engine->PostMusicSyncEvent(eventName->Data(), objectId, static_cast<AkUInt32>(0x202000)));
}
//Trigger on every musical bar
void WwiseSharpGameObject::PostMusicSyncEvent_Bar(Platform::String^ eventName)
{
	//void WwiseEngine::PostCallbackEvent(AkUniqueID eventId, AkGameObjectID gameObject, AkUInt32 uFlags = 0, AkCallbackFunc callback, void* pCookie)
	syncPlayingID = static_cast<uint32>(engine->PostMusicSyncEvent(eventName->Data(), objectId,  AK_EnableGetMusicPlayPosition));
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
void WwiseSharpGameObject::SetMultiplePositions(const Platform::Array<WwiseSharpTransform^>^ positions, uint16 numPositions)
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
void WwiseSharpGameObject::SetObjectRTPCValue(uint32 rtpcId, float32 value)
{
	engine->SetObjectRTPCValue(static_cast<AkRtpcID>(rtpcId), static_cast<AkRtpcValue>(value), objectId);
}
void WwiseSharpGameObject::SetObjectRTPCValue(Platform::String^ rtpcName, float32 value)
{
	engine->SetObjectRTPCValue(rtpcName->Data(), static_cast<AkRtpcValue>(value), objectId);
}
/*
void WwiseSharpGameObject::SetSwitch(uint32 switchGroupId, uint32 switchId)
{
	engine->SetSwitch(static_cast<AkSwitchGroupID>(switchGroupId), static_cast<AkSwitchStateID>(switchId), objectId);
}
*/
void WwiseSharpGameObject::SetSwitch(Platform::String^ switchGroupName, Platform::String^ switchName)
{
	engine->SetSwitch(switchGroupName->Data(), switchName->Data(), objectId);
}

void WwiseSharpGameObject::Seek(Platform::String^ eventName, uint32 position)
{
	engine->SeekOnEvent(eventName->Data(), objectId, position);
}

uint32 WwiseSharpGameObject::GetSourcePlayPosition(uint32 in_PlayingID)
{
	engine->GetSourcePlayPosition(static_cast<AkPlayingID>(in_PlayingID), out_puPosition);

	return static_cast<uint32>(*out_puPosition);
}

WwiseSharpMusicInfo WwiseSharpGameObject::GetPlayingSegmentInfo(uint32 in_PlayingID)
{
	AkSegmentInfo mSegmentInfo;
	WwiseSharpMusicInfo musicInfo;

	//null check

	mSegmentInfo = engine->GetPlayingSegmentInfo(static_cast<AkPlayingID>(in_PlayingID));

	if (mSegmentInfo.iCurrentPosition != NULL)
	{
		musicInfo.currentPosition = static_cast<uint32>(mSegmentInfo.iCurrentPosition);
		musicInfo.preEntryDuration = static_cast<uint32>(mSegmentInfo.iPreEntryDuration);
		musicInfo.activeDuration = static_cast<uint32>(mSegmentInfo.iActiveDuration);
		musicInfo.postExitDuration = static_cast<uint32>(mSegmentInfo.iPostExitDuration);
		musicInfo.remainingLookAheadTime = static_cast<uint32>(mSegmentInfo.iRemainingLookAheadTime);

		musicInfo.playingID = musicPlayingID;
		musicInfo.beatDuration = musicBeatDuration;
		musicInfo.barDuration = musicBarDuration;
		musicInfo.gridDuration = musicGridDuration;
		musicInfo.gridOffset = musicGridOffset;
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