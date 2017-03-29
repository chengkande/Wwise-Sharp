//////////////////////////////////////////////////////////////////////
//
// Info: Wwise 3D coordinates is left-handed with Y as Up.
//
//////////////////////////////////////////////////////////////////////


#ifndef WWISESHARP_WWISESHARPENGINE_HPP
#define WWISESHARP_WWISESHARPENGINE_HPP

#include "WwiseEngine.hpp"

#include "AkRoomVerbFXFactory.h"										// Room Verb
#include "SDK/include/AK/Plugin/AkPeakLimiterFXFactory.h"					//Limiter
#include "SDK/include/AK/Plugin/AkStereoDelayFXFactory.h"					//Stereo Delay for Bathroom effect
#include "SDK/include/AK/Plugin/AkVorbisDecoderFactory.h"					// Vorbis Plugin.
#include "SDK/include/AK/Plugin/AkMeterFXFactory.h"						//Meter, the best plugin

#include "WwiseSharpGameObject.hpp"
#include "Wwise_IDs.h"
#include "WwiseSharpTransform.h"

#ifndef WWISE_UWP
using namespace System::Collections::Generic;

namespace WwiseSharp
{
	public ref class WwiseSharpEngine
	{
	private:
		AkTransform *listenerPos;
	protected:
		WwiseWrapper::WwiseEngine* akengine;

	public:
		WwiseSharpEngine();
		virtual ~WwiseSharpEngine();
		//static StringToLPCWSTR^ pStringToLPCWSTR;
		//delegate LPCWSTR Converter(System::String^ str);

		bool Init();
		//LPCWSTR StringToLPCWSTRMethod(System::String^ str);
		void SetBasePath(System::String^ basePath);
		void SetAudioSrcPath(System::String^ basePath);
		void LoadBank(System::String^ bankName);
		void UnloadBank(System::String^ bankName);
		void PrepareBank(System::String^ bankName);
		System::String^ LoadEvents(array<System::String^>^ eventNames, unsigned short numEvents);
		void ClearBanks();
		void UnloadPreparedEvents(array<System::String^>^ eventNames, unsigned short numEvents);
		void ClearPreparedEvents();
		WwiseSharpGameObject^ RegisterGameObject(unsigned int objectId, System::String^ gameObjectLabel);

		void SetState(unsigned long stateGroupId, unsigned long stateId);
		void SetState(System::String^ stateGroup, System::String^ state);

		void SetGlobalRTPCValue(System::String^ rtpcName, float value);
		void SetGlobalRTPCValue(unsigned long rtpcId, float value);

		void SetListenerPosition(WwiseSharpTransform^ listenerPosition);
		void SetListenerPosition(int listenerIndex, WwiseSharpTransform^ listenerPosition);

		void SetPanType(bool type);
		void SetBusEffect(unsigned int fxID);
		void SetBusEffect2(unsigned int fxID);
		void InitReverb();

		void SetLanguage(System::String^ language);
		

		void Update();


		void UnregisterGameObject(WwiseSharpGameObject^ wgo);
		void UnregisterAllGameObject();
		void Term();
	};
}
#else
namespace WwiseSharp
{
	public ref class WwiseSharpEngine sealed
	{
	internal:
		AkTransform *listenerPos;
		AkVector *pos, *top, *front;
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

#endif