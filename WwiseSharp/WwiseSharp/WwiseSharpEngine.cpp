//////////////////////////////////////////////////////////////////////
//
//
// Important note about usage of System::String^ and StringToHGlobalUni heavily used in this file.
// At first, for your information, the ^ char means that the System::String is given as a managed object. It works like pointer.
// Because System::String is managed, it's hard to get a native char array. That's why I use a special interop method: StringToHGlobalUni which
// give a managed IntPtr. This one is managed but his role is to contain a native C pointer.
// Then I just need to cast it into LPCWSTR (const wchar*).
// Just after using it, we could (in fact we __must__ to avoid memory leaks) free the IntPtr to free up memory space.
//
//////////////////////////////////////////////////////////////////////


#include "WwiseSharpEngine.hpp"
using namespace WwiseSharp;
//delegate LPCWSTR Converter(System::String^ str);
#ifndef WWISE_UWP
WwiseSharpEngine::WwiseSharpEngine()
{
	akengine = new WwiseEngine();
	pos = new AkVector{ 0,0,0 };
	front = new AkVector{ 0,0,0 };
	top = new AkVector{ 0,0,0 };
	listenerPos->SetPosition(*pos);
	listenerPos->SetOrientation(*front, *top);
	//pStringToLPCWSTR = gcnew Array::Converter<System::String^, LPCWSTR>(StringToLPCWSTRMethod);
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
/*LPCWSTR StringToLPCWSTRMethod(System::String^ str)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str);
	return (static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
*/
void WwiseSharpEngine::SetBasePath(System::String^ basePath)
{
	//Because Wwise low level IO don't handle path without end '/' char, we need to add it in case it does'nt exists.
	if (basePath->default[basePath->Length - 1] != static_cast<wchar_t>('/'))
		basePath += "/";
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(basePath);
	akengine->SetBasePath(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
void WwiseSharpEngine::SetAudioSrcPath(System::String^ basePath)
{
	if (basePath->default[basePath->Length - 1] != static_cast<wchar_t>('/'))
		basePath += "/";
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(basePath);
	akengine->SetLooseMediaBasePath(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpEngine::LoadBank(System::String^ bankName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(bankName);
	akengine->LoadBank(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
void WwiseSharpEngine::UnloadBank(System::String^ bankName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(bankName);
	akengine->UnloadBank(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpEngine::PrepareBank(System::String^ bankName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(bankName);
	akengine->PrepareBank(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

System::String^ WwiseSharpEngine::LoadEvents(cli::array<System::String^>^ eventNames, unsigned short numEvents)
{
	LPCWSTR *eventArray = new LPCWSTR[numEvents];
	cli::array<System::IntPtr>^ pa = gcnew array<System::IntPtr>(numEvents);
	for(unsigned short x = 0; x < numEvents; x++)
	{
		//eventArray[x] = new wchar_t[50]; //allocate memory for each
		pa[x] = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventNames[x]);
		eventArray[x] = static_cast<LPCWSTR>(static_cast<void *>(pa[x]));
		
	}
	pin_ptr<LPCWSTR> pp = &eventArray[0];
	System::String^ msg = gcnew System::String(akengine->LoadEvent(pp, numEvents)); //THIS PART WORKS SO FAR
	
	delete eventArray;
	return msg;

	//TODO: DEALLOCATE INTPTR ARRAY AND EVENTARRAY, COPY TO CHANGES TO OTHER METHODS, TEST, CRY A LOT
}
void WwiseSharpEngine::ClearBanks()
{
	akengine->ClearBanks();
}
void WwiseSharpEngine::UnloadPreparedEvents(array<System::String^>^ eventNames, unsigned short numEvents)
{

	LPCWSTR * eventArray = new LPCWSTR[numEvents];
	cli::array<System::IntPtr>^ pa = gcnew array<System::IntPtr>(numEvents);
	for (unsigned short x = 0; x < numEvents; x++)
	{
		pa[x] = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventNames[x]);
		eventArray[x] = static_cast<LPCWSTR>(static_cast<void *>(pa[x]));
		
	}
	pin_ptr<LPCWSTR> pp = &eventArray[0];
	akengine->UnloadPreparedEvent(pp, numEvents);
	delete eventArray;
}
void WwiseSharpEngine::ClearPreparedEvents()
{
	akengine->ClearPreparedEvents();
}
WwiseSharpGameObject^ WwiseSharpEngine::RegisterGameObject(unsigned int objectId, System::String^ gameObjectLabel)
{
	WwiseSharpGameObject^ wgo = gcnew WwiseSharpGameObject(objectId, gameObjectLabel, akengine);
	akengine->RegisterGameObject(wgo->GetAkObjectId(), wgo->GetLabelChar());
	return wgo;
}

void WwiseSharpEngine::SetState(unsigned long stateGroupId, unsigned long stateId)
{
	akengine->SetState(static_cast<AkStateGroupID>(stateGroupId), static_cast<AkStateID>(stateId));
}
void WwiseSharpEngine::SetState(System::String^ stateGroup, System::String^ state)
{
	System::IntPtr p1 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(stateGroup);
	System::IntPtr p2 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(state);
	akengine->SetState(static_cast<LPCWSTR>(static_cast<void *>(p1)), static_cast<LPCWSTR>(static_cast<void *>(p2)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p1);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p2);
}

//Set RTPC value globally.  Will not override Set Game Parameter events. Use for in-game audio options, busses, Daryl Health, etc
void WwiseSharpEngine::SetGlobalRTPCValue(unsigned long rtpcId, float value)
{
	akengine->SetGlobalRTPCValue(static_cast<AkRtpcID>(rtpcId), static_cast<AkRtpcValue>(value));
}
void WwiseSharpEngine::SetGlobalRTPCValue(System::String^ rtpcName, float value)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(rtpcName);
	akengine->SetGlobalRTPCValue(static_cast<LPCWSTR>(static_cast<void *>(p)), static_cast<AkRtpcValue>(value));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpEngine::SetListenerPosition(WwiseSharpTransform^ listenerPosition)
{
	pos->X = listenerPosition->Xposition;
	pos->Y = listenerPosition->Yposition;
	pos->Z = listenerPosition->Zposition;

	listenerPos->SetPosition(*pos);

	akengine->SetListenerPosition(*listenerPos);
}
void WwiseSharpEngine::SetListenerPosition(int listenerIndex, WwiseSharpTransform^ listenerPosition)
{
	pos->X = listenerPosition->Xposition;
	pos->Y = listenerPosition->Yposition;
	pos->Z = listenerPosition->Zposition;

	listenerPos->SetPosition(*pos);

	akengine->SetListenerPosition(listenerIndex, *listenerPos);
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
void WwiseSharpEngine::SetBusEffect(unsigned int fxID)
{
	akengine->SetBusEffect(348963605, (AkUInt32)0, (AkUniqueID)fxID);
}
void WwiseSharpEngine::SetBusEffect2(unsigned int fxID)
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

void WwiseSharpEngine::SetLanguage(System::String^ language)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(language);
	akengine->SetLanguage(static_cast<LPCWSTR>(static_cast<void *>(p)));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}




#else
/////////////////////////////////////////////////////////////
///////--------------- U W P ---------------////////////////
////////////////////////////////////////////////////////////

WwiseSharpEngine::WwiseSharpEngine()
{
	akengine = new WwiseEngine();
	pos = new AkVector{ 0,0,0 };
	front = new AkVector{ 0,0,0 };
	top = new AkVector{ 0,0,0 };
	listenerPos->SetPosition(*pos);
	listenerPos->SetOrientation(*front, *top);
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
	pos->X = listenerPosition->Xposition;
	pos->Y = listenerPosition->Yposition;
	pos->Z = listenerPosition->Zposition;

	listenerPos->SetPosition(*pos);

	akengine->SetListenerPosition(*listenerPos);
}
void WwiseSharpEngine::SetListenerPosition(uint16 listenerIndex, WwiseSharpTransform^ listenerPosition)
{
	pos->X = listenerPosition->Xposition;
	pos->Y = listenerPosition->Yposition;
	pos->Z = listenerPosition->Zposition;

	listenerPos->SetPosition(*pos);

	akengine->SetListenerPosition(listenerIndex, *listenerPos);
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
#endif