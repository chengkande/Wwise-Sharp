#include <Windows.h>
#include "pch.h"
//////////////////////////////////////////////////////////////////////
//
//
// Important note about usage of Platform::String^ and StringToHGlobalUni heavily used in this file.
// At first, for your information, the ^ char means that the Platform::String is given as a managed object. It works like pointer.
// Because Platform::String is managed, it's hard to get a native char array. That's why I use a special interop method: StringToHGlobalUni which
// give a managed IntPtr. This one is managed but his role is to contain a native C pointer.
// Then I just need to cast it into LPCWSTR (const wchar*).
// Just after using it, we could (in fact we __must__ to avoid memory leaks) free the IntPtr to free up memory space.
//
//////////////////////////////////////////////////////////////////////
//#include "SDK/include/AK/Tools/Win32/ThreadEmulation.h"
#include "WwiseSharpEngine.hpp"
using namespace WwiseSharp;
//delegate LPCWSTR Converter(Platform::String^ str);

WwiseSharpEngine::WwiseSharpEngine()
{
	akengine = new WwiseEngine();
	//pStringToLPCWSTR = gcnew Array::Converter<Platform::String^, LPCWSTR>(StringToLPCWSTRMethod);
}

WwiseSharpEngine::~WwiseSharpEngine()
{
	delete akengine;
}

bool WwiseSharpEngine::Init()
{
	bool status = akengine->Init();
	SetPanType(true);
	return status;
}
/*LPCWSTR StringToLPCWSTRMethod(Platform::String^ str)
{
System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str);
return (static_cast<LPCWSTR>(static_cast<void *>(p)));
System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
*/
void WwiseSharpEngine::SetBasePath(Platform::String^ basePath)
{
	//Because Wwise low level IO don't handle path without end '/' char, we need to add it in case it does'nt exists.
	if (basePath->Data()[basePath->Length() - 1] != static_cast<wchar_t>('/'))
		basePath += "/";

	//may not need marshalling anymore?!!?!?
	//System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(basePath);
	akengine->SetBasePath(basePath->Data());
	//System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
void WwiseSharpEngine::SetAudioSrcPath(Platform::String^ basePath)
{
	if (basePath->Data()[basePath->Length() - 1] != static_cast<wchar_t>('/'))
		basePath += "/";
	akengine->SetLooseMediaBasePath(basePath->Data());

}

void WwiseSharpEngine::LoadBank(Platform::String^ bankName)
{
	akengine->LoadBank(bankName->Data());
}
void WwiseSharpEngine::UnloadBank(Platform::String^ bankName)
{
	akengine->UnloadBank(bankName->Data());
}

void WwiseSharpEngine::PrepareBank(Platform::String^ bankName)
{
	akengine->PrepareBank(bankName->Data());
}

void WwiseSharpEngine::LoadEvents(const Platform::Array<Platform::String^>^ eventNames, uint16 numEvents)
{
	LPCWSTR *eventArray = new LPCWSTR[numEvents];
	//Platform::Array<System::IntPtr>^ pa = gcnew array<System::IntPtr>(numEvents);
	for (uint16 x = 0; x < numEvents; x++)
	{
		//eventArray[x] = new wchar_t[50]; //allocate memory for each
		//pa[x] = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventNames[x]);
		eventArray[x] = eventNames[x]->Data();

	}
	LPCWSTR* pp = &eventArray[0];
	akengine->LoadEvent(pp, numEvents);

	delete eventArray;
}
void WwiseSharpEngine::ClearBanks()
{
	akengine->ClearBanks();
}
void WwiseSharpEngine::UnloadPreparedEvents(const Platform::Array<Platform::String^>^ eventNames, uint16 numEvents)
{
	LPCWSTR * eventArray = new LPCWSTR[numEvents];
	for (uint16 x = 0; x < numEvents; x++)
	{
		eventArray[x] = eventNames[x]->Data();
	}
	LPCWSTR* pp = &eventArray[0];
	akengine->UnloadPreparedEvent(pp, numEvents);
	delete eventArray;
}
void WwiseSharpEngine::ClearPreparedEvents()
{
	akengine->ClearPreparedEvents();
}
WwiseSharpGameObject^ WwiseSharpEngine::RegisterGameObject(uint32 objectId, Platform::String^ gameObjectLabel)
{
	WwiseSharpGameObject^ wgo = ref new WwiseSharpGameObject(objectId, gameObjectLabel, akengine);

	Platform::String^ wwiseLabel = wgo->GetLabelChar();
	const wchar_t* w = wwiseLabel->Data();
	size_t i;
	char *c = new char[wgo->GetLabelChar()->Length() + 1];
	memset(c, 0, wgo->GetLabelChar()->Length() + 1);
	wcstombs_s(&i, c, (size_t)wwiseLabel->Length() + 1, wwiseLabel->Data(), (size_t)wwiseLabel->Length() + 1);

	akengine->RegisterGameObject(wgo->GetAkObjectId(), c);
	return wgo;
}
[Windows::Foundation::Metadata::DefaultOverload]
void WwiseSharpEngine::SetState(Platform::String^ stateGroup, Platform::String^ state)
{
	akengine->SetState(stateGroup->Data(), state->Data());
}
void WwiseSharpEngine::SetState(uint32 stateGroupId, uint32 stateId)
{
	akengine->SetState(static_cast<AkStateGroupID>(stateGroupId), static_cast<AkStateID>(stateId));
}


//Set RTPC value globally.  Will not override Set Game Parameter events. Use for in-game audio options, busses, Daryl Health, etc
[Windows::Foundation::Metadata::DefaultOverload]
void WwiseSharpEngine::SetGlobalRTPCValue(Platform::String^ rtpcName, float32 value)
{
	akengine->SetGlobalRTPCValue(rtpcName->Data(), static_cast<AkRtpcValue>(value));
}
void WwiseSharpEngine::SetGlobalRTPCValue(uint32 rtpcId, float32 value)
{
	akengine->SetGlobalRTPCValue(static_cast<AkRtpcID>(rtpcId), static_cast<AkRtpcValue>(value));
}


void WwiseSharpEngine::SetListenerPosition(WwiseSharpTransform^ listenerPosition)
{
	AkVector akPosition;
	akPosition.X = listenerPosition->Xposition;
	akPosition.Y = listenerPosition->Yposition;
	akPosition.Z = listenerPosition->Zposition;

	AkVector akOrientationFront{ 0,0,1 };
	AkVector akOrientationTop{ 0,1,0 };

	AkTransform akListenerPosition;
	akListenerPosition.SetPosition(akPosition);
	akListenerPosition.SetOrientation(akOrientationFront, akOrientationTop);

	akengine->SetListenerPosition(akListenerPosition);
}
void WwiseSharpEngine::SetListenerPosition(uint16 listenerIndex, WwiseSharpTransform^ listenerPosition)
{
	AkVector akPosition;
	akPosition.X = listenerPosition->Xposition;
	akPosition.Y = listenerPosition->Yposition;
	akPosition.Z = listenerPosition->Zposition;

	AkVector akOrientationFront{ 0,0,1 };
	AkVector akOrientationTop{ 0,1,0 };

	AkTransform akListenerPosition;
	akListenerPosition.SetPosition(akPosition);
	akListenerPosition.SetOrientation(akOrientationFront, akOrientationTop);

	akengine->SetListenerPosition(listenerIndex, akListenerPosition);
}
//true = headphones, false = speakers
void WwiseSharpEngine::SetPanType(bool type)
{
	if (type)
	{
		akengine->SetPanningRule(AkPanningRule_Headphones);
	}
	else
	{
		akengine->SetPanningRule(AkPanningRule_Speakers);
	}
}
//only controls Reverb bus fx right now
void WwiseSharpEngine::SetBusEffect(uint32 fxID)
{
	akengine->SetBusEffect(348963605, (AkUInt32)0, (AkUniqueID)fxID);
}
void WwiseSharpEngine::SetBusEffect2(uint32 fxID)
{
	akengine->SetBusEffect(35548098, (AkUInt32)0, (AkUniqueID)fxID);
}

void WwiseSharpEngine::InitReverb()
{
	//DO NOTHING
}

void WwiseSharpEngine::Update()
{
	akengine->ProcessAudio();
}

void WwiseSharpEngine::UnregisterGameObject(WwiseSharpGameObject^ wgo)
{
	akengine->UnregisterGameObject(wgo->GetAkObjectId());
}

void WwiseSharpEngine::UnregisterAllGameObject()
{
	akengine->UnregisterAllGameObject();
}

void WwiseSharpEngine::Term()
{
	akengine->Term();
}

void WwiseSharpEngine::SetLanguage(Platform::String^ language)
{
	akengine->SetLanguage(language->Data());
}
