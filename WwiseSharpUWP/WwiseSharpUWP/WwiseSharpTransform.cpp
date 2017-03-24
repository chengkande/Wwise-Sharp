//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "WwiseSharpTransform.h"

using namespace WwiseSharp;

WwiseSharpTransform::WwiseSharpTransform(float x, float y, float z)
{
	Xposition = x;
	Yposition = y;
	Zposition = z;
}

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