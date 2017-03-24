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

#include "CarParams.h"

class Car
{
public:
	Car();
	virtual ~Car();

	void SetCarParams( const CarParams& in_rCarParam );
	const CarParams& GetCarParam() const;
	CarParams& GetCarParam();

	// Set the car param to the default values
	void SetDefaultCarParam();

	// Return the engine force (in Newton) on the ground
	double GetCarEngineForce() const;

	// return a negative force in newton representing the air drag on the car
	double GetDragForce() const;

	// return a negative force in newton 
	double GetFrictionForce() const;

	// Get the RPM function of the speed set previously.
	double GetRPM() const;

	// set Throttle in a range of -1 to 1
	// < 0 to simulate break ....
	void SetThrottle( const double& in_rThrottle );
	double GetThrottle() const;

	// set speed in m/s
	void SetSpeed( const double& in_rSpeed );
	double GetSpeed() const;

	// Set load in a range of -1 to 1
	void SetLoad( const double& in_rLoad );
	double GetLoad() const;

	// 0- based gear index ( 0 for first gear, 1 for second, etc... )
	void SetCurrentGear( size_t in_currentGear );
	size_t GetCurrentGear() const;

	void ResetSimulationData();

private:

	CarParams m_carParams;

	// Current State
	double m_dblThrottle;			// from -1 to 1
	double m_dblSpeed;				// M/s
	double m_dblLoad;
	size_t m_nCurrentGear;			// 0-based
};
