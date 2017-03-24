#pragma once
#include "pch.h"
//////////////////////////////////////////////////////////////////////
//
// Info: Wwise 3D coordinates is left-handed with Y as Up.
//
//////////////////////////////////////////////////////////////////////


#ifndef WWISESHARP_WWISESHARPENGINE_HPP
#define WWISESHARP_WWISESHARPENGINE_HPP

#include "WwiseEngine.hpp"

#include "SDK/include/AK/Plugin/AkRoomVerbFXFactory.h"						// Room Verb
#include "SDK/include/AK/Plugin/AkPeakLimiterFXFactory.h"					//Limiter
#include "SDK/include/AK/Plugin/AkStereoDelayFXFactory.h"					//Stereo Delay for Bathroom effect
#include "SDK/include/AK/Plugin/AkVorbisDecoderFactory.h"					// Vorbis Plugin.
#include "SDK/include/AK/Plugin/AkMeterFXFactory.h"						//Meter, the best plugin

#include "WwiseSharpGameObject.hpp"

//#include "Wwise_IDs.h"
#include "WwiseSharpTransform.h"

namespace WwiseSharp
{
	public ref class WwiseSharpEngine sealed
	{
	internal:
		WwiseWrapper::WwiseEngine* akengine;

	public:
		WwiseSharpEngine();
		virtual ~WwiseSharpEngine();
		//static StringToLPCWSTR^ pStringToLPCWSTR;
		//delegate LPCWSTR Converter(Platform::String^ str);

		bool Init();
		//LPCWSTR StringToLPCWSTRMethod(Platform::String^ str);
		void SetBasePath(Platform::String^ basePath);
		void SetAudioSrcPath(Platform::String^ basePath);
		void LoadBank(Platform::String^ bankName);
		void UnloadBank(Platform::String^ bankName);
		void PrepareBank(Platform::String^ bankName);
		void LoadEvents(const Platform::Array<Platform::String^>^ eventNames, uint16 numEvents);
		void ClearBanks();
		void UnloadPreparedEvents(const Platform::Array<Platform::String^>^ eventNames, uint16 numEvents);
		void ClearPreparedEvents();
		WwiseSharpGameObject^ RegisterGameObject(uint32 objectId, Platform::String^ gameObjectLabel);

		void SetState(uint32 stateGroupId, uint32 stateId);
		void SetState(Platform::String^ stateGroup, Platform::String^ state);

		void SetGlobalRTPCValue(Platform::String^ rtpcName, float32 value);
		void SetGlobalRTPCValue(uint32 rtpcId, float32 value);

		void SetListenerPosition(WwiseSharpTransform^ listenerPosition);
		void SetListenerPosition(uint16 listenerIndex, WwiseSharpTransform^ listenerPosition);

		void SetPanType(bool type);
		void SetBusEffect(uint32 fxID);
		void SetBusEffect2(uint32 fxID);
		void InitReverb();

		void SetLanguage(Platform::String^ language);


		void Update();


		void UnregisterGameObject(WwiseSharpGameObject^ wgo);
		void UnregisterAllGameObject();
		void Term();
	};
}

#endif