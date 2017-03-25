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

#include "ICarSimUI.h"
#include "CarSimAssignment.h"
#include "CarSimComboBox.h"

#include <AK/SoundFrame/SF.h>

using namespace AK;
using namespace SoundFrame;

class CarSimAssignmentDlg : public CDialog,
							public ICarSimUI
{
public:
	CarSimAssignmentDlg( CWnd* pParent = NULL);

	// ICarSimUI
	virtual void OnSimulationSettingsChanged();
	virtual void OnNotif( ObjectType in_eObjectType, IClient::Notif in_eNotif, AkUniqueID in_objectID );
	virtual void UpdateFrame();

	// CarSimAssignmentDlg
	void Init( const ISoundFrame * in_pSoundFrame );

	void SetAssignment( const CarSimAssignment& in_rAssignment );
	const CarSimAssignment& GetAssignment() const;

	// Dialog Data
	enum { IDD = IDD_ASSIGNEMENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:

	CarSimAssignmentComboBox<IGameParameter, IGameParameterList> m_rpmCombo;
	CarSimAssignmentComboBox<IGameParameter, IGameParameterList> m_loadCombo;
	CarSimAssignmentComboBox<IGameParameter, IGameParameterList> m_speedCombo;

	CarSimAssignmentComboBox<IEvent, IEventList> m_eventPlayCombo;
	CarSimAssignmentComboBox<IEvent, IEventList> m_eventStopCombo;
	CarSimAssignmentComboBox<IEvent, IEventList> m_eventUpShiftCombo;
	CarSimAssignmentComboBox<IEvent, IEventList> m_eventDownShiftCombo;

	CarSimAssignmentComboBox<IAuxBus, IAuxBusList> m_AuxBusCombo;

	CarSimAssignment m_assignment;

	const ISoundFrame * m_pSoundFrame;
};