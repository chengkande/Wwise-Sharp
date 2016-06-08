//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
// -------------------------------------------------------------------
//
// Info: Wwise 3D coordinates is left-handed with Y as Up.
//
//////////////////////////////////////////////////////////////////////


#ifndef WWISESHARP_WWISESHARPENGINE_HPP
#define WWISESHARP_WWISESHARPENGINE_HPP

#include "WwiseEngine.hpp"

#include "WwiseSharpGameObject.hpp"

#include "WwiseSharpListenerPosition.hpp"
using namespace System::Collections::Generic;

namespace WwiseSharp
{
	public ref class WwiseSharpEngine
	{
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

		void SetListenerPosition(WwiseSharpListenerPosition^ listenerPosition);
		void SetListenerPosition(int listenerIndex, WwiseSharpListenerPosition^ listenerPosition);

		void SetPanType(bool type);
		void SetBusEffect(unsigned int fxID);
		void InitReverb();
		

		void Update();


		void UnregisterGameObject(WwiseSharpGameObject^ wgo);
		void UnregisterAllGameObject();
		void Term();
	};
}

#endif