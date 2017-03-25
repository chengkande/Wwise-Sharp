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
	//ignore this shit in the constructor as we're not using it
}
void WwiseSharpTransform::SetPosition(float x, float y)
{
	Xposition = x;
	Yposition = y;
}