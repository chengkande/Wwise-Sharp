#ifndef WWISESHARP_WWISESHARPMUSICINFO_HPP
#define WWISESHARP_WWISESHARPMUSICINFO_HPP

namespace WwiseSharp
{
#ifndef WWISE_UWP
	public ref struct WwiseSharpMusicInfo
	{
	public:

		unsigned long currentPosition, preEntryDuration, activeDuration, postExitDuration, remainingLookAheadTime;
		unsigned long playingID, beatDuration, barDuration, gridDuration, gridOffset;

	};
#else //UWP
	public value struct WwiseSharpMusicInfo sealed
	{
		uint32 currentPosition, preEntryDuration, activeDuration, postExitDuration, remainingLookAheadTime;
		uint32 playingID, beatDuration, barDuration, gridDuration, gridOffset;
	};
#endif
}
#endif