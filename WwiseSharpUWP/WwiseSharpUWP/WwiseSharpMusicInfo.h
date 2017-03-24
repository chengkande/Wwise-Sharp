#include "pch.h"
#ifndef WWISESHARP_WWISESHARPMUSICINFO_HPP
#define WWISESHARP_WWISESHARPMUSICINFO_HPP

namespace WwiseSharp
{
	public value struct WwiseSharpMusicInfo sealed
	{
		uint32 currentPosition, preEntryDuration, activeDuration, postExitDuration, remainingLookAheadTime;
		uint32 playingID, beatDuration, barDuration, gridDuration, gridOffset;
	};
}

#endif