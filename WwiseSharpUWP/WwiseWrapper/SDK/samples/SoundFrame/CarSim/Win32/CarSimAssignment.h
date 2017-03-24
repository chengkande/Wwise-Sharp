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

class CarSimAssignment
{
public:
	// Default constructor ...
	CarSimAssignment();
	~CarSimAssignment();

	void Clear();
	void Assign( const CarSimAssignment& in_rAssignement );

	void RemoveObject( AkUniqueID in_objectID );

	AkUniqueID m_RPMParam;
	AkUniqueID m_LoadParam;
	AkUniqueID m_SpeedParam;

	AkUniqueID m_EnginePlayEvent;
	AkUniqueID m_EngineStopEvent;
	AkUniqueID m_UpShiftEvent;
	AkUniqueID m_DownShiftEvent;

	AkUniqueID m_AuxBus;

private:
	CarSimAssignment(const CarSimAssignment&);
	CarSimAssignment& operator=( const CarSimAssignment& );
};