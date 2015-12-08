#ifndef WWISESHARP_WWISESHARPMUSICINFO_HPP
#define WWISESHARP_WWISESHARPMUSICINFO_HPP

namespace WwiseSharp
{
	public ref struct WwiseSharpMusicInfo
	{
	public:
		//float Xposition, Yposition, Zposition;
		//float Xorientation, Yorientation, Zorientation;

		unsigned long currentPosition, preEntryDuration, activeDuration, postExitDuration, remainingLookAheadTime;
		unsigned long playingID, beatDuration, barDuration, gridDuration, gridOffset;

	};
}

#endif