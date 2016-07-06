//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#include "WwiseSharpTransform.h"
using namespace WwiseSharp;

WwiseSharpTransform::WwiseSharpTransform(float x, float y, float z)
{
	Xposition = x;
	Yposition = y;
	Zposition = z;
	//ignore this shit in the constructor if we're not using it
	XorientationFront = 0.0f;
	YorientationFront = 1.0f;
	ZorientationFront = 0.0f;
	
	XorientationTop = 0.0f;
	YorientationTop = 1.0f;
	ZorientationTop = 0.0f;
}
//override if you actually want to use it
WwiseSharpTransform::WwiseSharpTransform(float x, float y, float z, float xF, float yF, float zF, float xT, float yT, float zT)
{
	Xposition = x;
	Yposition = y;
	Zposition = z;

	XorientationFront = xF;
	YorientationFront = yF;
	ZorientationFront = zF;

	XorientationTop = xT;
	YorientationTop = yT;
	ZorientationTop = zT;
}