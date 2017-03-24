//////////////////////////////////////////////////////////////////////
//
//
//
// -------------------------------------------------------------------
//
// /!\ Important Use Note: refering to the Wwise documentation (Integrating Listeners), OrientationFront & OrientationTop cannot be Zero vector.
// The OrientationFront vector defines the direction that the listener's head is facing. It should be orthogonal to the OrientationTop vector, which defines the incline of the listener's head. For a human listener, one could think of the OrientationFront vector as the listener's nose (going away from the face), while the OrientationTop vector would be orthogonal to it, going between the listener's eyes and away from the chin. 
// The orientation vectors must be defined for the audio to be rendered properly. They cannot be zero vectors and should be unit vectors; if not they will be converted to unit vectors. They should be at right angles. If not, the front orientation vector will be adjusted.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "pch.h"


namespace WwiseSharp
{
	public ref struct WwiseSharpTransform sealed
	{
	internal:
		float32 Xposition, Yposition, Zposition;
		float32 XorientationFront, YorientationFront, ZorientationFront;
		float32 XorientationTop, YorientationTop, ZorientationTop;
	public:
		WwiseSharpTransform(float x, float y, float z);

		WwiseSharpTransform(float x, float y, float z, float xF, float yF, float zF, float xT, float yT, float zT);
	};
}