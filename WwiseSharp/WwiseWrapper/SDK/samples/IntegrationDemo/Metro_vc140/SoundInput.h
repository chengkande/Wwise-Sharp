/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: v2016.2.2  Build: 6021
  Copyright (c) 2006-2017 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include "SoundInputBase.h"

class SoundInput : public SoundInputBase
{
public:
	SoundInput() {}
	~SoundInput() {}

	static SoundInput& Instance()
	{
		return ms_Instance;
	}

	virtual bool InputOn( unsigned int /*in_DevNumber*/ = 0 ) { return false;}		// Start input recording
	virtual bool InputOff() { return false; }									// Stop input recording

	virtual void Execute( AkAudioBuffer* /*io_pBufferOut*/ ) {}
	virtual void GetFormatCallback( AkAudioFormat&  /*io_AudioFormat*/ ) {}

private:
	static SoundInput ms_Instance;

};
