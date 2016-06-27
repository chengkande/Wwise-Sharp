#ifndef WWISESHARP_WWISESHARPMUSICINFO_HPP
#define WWISESHARP_WWISESHARPMUSICINFO_HPP

namespace WwiseSharp
{
	public ref struct WwiseSharpMusicInfo
	{
	public:

		unsigned long currentPosition, preEntryDuration, activeDuration, postExitDuration, remainingLookAheadTime;
		unsigned long playingID, beatDuration, barDuration, gridDuration, gridOffset;

	};
}

#endif