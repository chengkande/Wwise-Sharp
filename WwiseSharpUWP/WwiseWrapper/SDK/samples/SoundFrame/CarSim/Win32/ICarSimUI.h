/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: v2016.2.2  Build: 6022
  Copyright (c) 2006-2017 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include <AK/SoundFrame/SF.h>

using namespace AK;
using namespace SoundFrame;

class ICarSimUI
{
public:
	enum ObjectType
	{
		Type_Event,
		Type_GameParameter,
		Type_AuxBus
	};

	// Tell the that the simulation settings have been changed
	// Usually when you load a simulation file or when you connected or disconnected from Wwise.
	virtual void OnSimulationSettingsChanged() = 0;

	// Called when an object notification is sent by the Sound frame
	virtual void OnNotif( ObjectType in_eObjectType, IClient::Notif in_eNotif, AkUniqueID in_objectID ) = 0;

	// Called each frame to tell the UI to update the simulation states
	virtual void UpdateFrame() = 0;
};