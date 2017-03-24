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

// DemoMusicCallbacks.cpp
/// \file 
/// Defines the methods declared in DemoMarkers.h.

#include "stdafx.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine

#include "../WwiseProject/GeneratedSoundBanks/Wwise_IDs.h"		// IDs generated by Wwise
#include "Menu.h"
#include "DemoMusicCallbacks.h"


static const int kNotesPerOctave = 12;
static const char* kNoteArray[kNotesPerOctave] = 
	{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

// Helper to convert note number to note text
void MidiNoteToString( int in_iNoteNum, char* out_szNote, unsigned int in_uiBufferSize )
{
	int octave = (in_iNoteNum/kNotesPerOctave);
	const char* myNote = kNoteArray[ in_iNoteNum % kNotesPerOctave ];

	octave -= 1;
	snprintf( out_szNote, in_uiBufferSize, "%s%i", myNote, octave);
}

// *********************************************************************************
// Root Page
// *********************************************************************************

DemoMusicCallbacksRoot::DemoMusicCallbacksRoot( Menu& in_ParentMenu )
: Page( in_ParentMenu, "Music Callbacks Demos" )
{
	m_szHelp  = "This demo shows how to use the different music callbacks.\n\n";
}

void DemoMusicCallbacksRoot::InitControls()
{
	ButtonControl* newBtn;

	newBtn = new ButtonControl( *this );
	newBtn->SetLabel( "Music Sync Callback Demo" );
	newBtn->SetDelegate( (PageMFP)&DemoMusicCallbacksRoot::MusicSyncCallbackButton_Pressed );
	m_Controls.push_back( newBtn );

	newBtn = new ButtonControl( *this );
	newBtn->SetLabel( "Music Playlist Callback Demo" );
	newBtn->SetDelegate( (PageMFP)&DemoMusicCallbacksRoot::MusicPlaylistCallbackButton_Pressed );
	m_Controls.push_back( newBtn );

	newBtn = new ButtonControl( *this );
	newBtn->SetLabel( "MIDI Callback Demo" );
	newBtn->SetDelegate( (PageMFP)&DemoMusicCallbacksRoot::MusicMIDICallbackButton_Pressed );
	m_Controls.push_back( newBtn );
}

void DemoMusicCallbacksRoot::MusicSyncCallbackButton_Pressed( void* , ControlEvent* )
{
	DemoMusicCallbacks* pg = new DemoMusicCallbacks( *m_pParentMenu );
	m_pParentMenu->StackPage( pg );
}

void DemoMusicCallbacksRoot::MusicPlaylistCallbackButton_Pressed( void* , ControlEvent* )
{
	DemoMusicPlaylistCallbacks* pg = new DemoMusicPlaylistCallbacks( *m_pParentMenu );
	m_pParentMenu->StackPage( pg );
}

void DemoMusicCallbacksRoot::MusicMIDICallbackButton_Pressed( void* , ControlEvent* )
{
	DemoMIDICallbacks* pg = new DemoMIDICallbacks( *m_pParentMenu );
	m_pParentMenu->StackPage( pg );
}

// *********************************************************************************
// Base class
// *********************************************************************************

DemoMusicCallbacksBase::DemoMusicCallbacksBase( Menu& in_ParentMenu, const string& in_strPageTitle )
: Page( in_ParentMenu, in_strPageTitle )
, m_bIsPlaying( false )
, m_bStopPlaylist( false )
, m_iPlayingID( 0 )
{
}

bool DemoMusicCallbacksBase::Init()
{
	// Load the sound bank
	AkBankID bankID; // Not used
	if ( AK::SoundEngine::LoadBank( "MusicCallbacks.bnk", AK_DEFAULT_POOL_ID, bankID ) != AK_Success )
	{
		SetLoadFileErrorMessage( "MusicCallbacks.bnk" );
		return false;
	}

	// Register the "Markers" game object
	AK::SoundEngine::RegisterGameObj( GAME_OBJECT_MUSIC, "Music" );

	// Initialize the marker playback variables
	m_bIsPlaying = true;
	
	// Initialize the page
	return Page::Init();
}

void DemoMusicCallbacksBase::Release()
{
	// Make sure we do not get called back after page destruction
	AK::SoundEngine::CancelEventCallback( m_iPlayingID );

	// Stop the sound playing
	AK::SoundEngine::StopPlayingID( m_iPlayingID );

	// Unregister the "Music" game object
	AK::SoundEngine::UnregisterGameObj( GAME_OBJECT_MUSIC );

	// Unload the sound bank
	AK::SoundEngine::UnloadBank( "MusicCallbacks.bnk", NULL );

	// Releases the page
	Page::Release();
}

void DemoMusicCallbacksBase::Draw()
{
	Page::Draw();

	if ( ! m_bIsPlaying )
	{
		int iPosX = m_pParentMenu->GetWidth() / 4;
		int iPosY = m_pParentMenu->GetHeight() / 3;
		DrawTextOnScreen( "Test Finished", iPosX, iPosY, DrawStyle_Text );	
	}
}

// *********************************************************************************
// Music Callbacks
// *********************************************************************************

DemoMusicCallbacks::DemoMusicCallbacks( Menu& in_ParentMenu )
: DemoMusicCallbacksBase( in_ParentMenu, "Music Sync Callback Demo" )
, m_uiBeatCount( 0 )
, m_uiBarCount( 0 )
{
	m_szHelp  = "Beat and Bar notifications are generated from "
				"playing music's tempo and time signature info.";
}


void DemoMusicCallbacks::Draw()
{
	DemoMusicCallbacksBase::Draw();

	if ( m_bIsPlaying )
	{
		char strBuf[50];
		int iPosX = m_pParentMenu->GetWidth() / 4;
		int iPosY = m_pParentMenu->GetHeight() / 3;

		AkTimeMs uPosition;

		// Get the current play position and store it in a string buffer
		AK::SoundEngine::GetSourcePlayPosition( m_iPlayingID, &uPosition );
		snprintf( strBuf, 50, "Bar: %d\nBeat: %d\nPosition=%d", (int)m_uiBarCount, (int)m_uiBeatCount, (int)uPosition );

		// Draw the play position and subtitles
		DrawTextOnScreen( strBuf, iPosX, iPosY, DrawStyle_Text );
	}
}

bool DemoMusicCallbacks::Init()
{
	DemoMusicCallbacksBase::Init();

	m_iPlayingID = AK::SoundEngine::PostEvent( 
		AK::EVENTS::PLAYMUSICDEMO1,
		GAME_OBJECT_MUSIC,
		AK_EnableGetSourcePlayPosition | AK_MusicSyncBeat | AK_MusicSyncBar | AK_MusicSyncEntry | AK_MusicSyncExit | AK_EndOfEvent,
		&DemoMusicCallbacks::MusicCallback,
		this
		);

	return true;
}

void DemoMusicCallbacks::MusicCallback( AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo )
{
	DemoMusicCallbacks* pPage = (DemoMusicCallbacks*)in_pCallbackInfo->pCookie;
	
	if ( in_eType == AK_MusicSyncBar )
	{
		pPage->m_uiBeatCount = 0;
		pPage->m_uiBarCount++;
	}
	else if ( in_eType == AK_MusicSyncBeat )
	{
		pPage->m_uiBeatCount++;
	}
	else if ( in_eType == AK_EndOfEvent )
	{
		pPage->m_bIsPlaying = false;
		pPage->m_uiBeatCount = 0;
		pPage->m_uiBarCount = 0;
	}
}

// *********************************************************************************
// Playlist Callbacks
// *********************************************************************************

DemoMusicPlaylistCallbacks::DemoMusicPlaylistCallbacks( Menu& in_ParentMenu )
: DemoMusicCallbacksBase( in_ParentMenu, "Music Playlist Callback Demo" )
, m_uiPlaylistItem( 0 )
{
	m_szHelp  = "This example to force a random playlist to select its next item sequentially. "
				"The playlist item may be stopped via the callback as well.";
}


void DemoMusicPlaylistCallbacks::Draw()
{
	DemoMusicCallbacksBase::Draw();

	if ( m_bIsPlaying )
	{
		char strBuf[50];
		int iPosX = m_pParentMenu->GetWidth() / 4;
		int iPosY = m_pParentMenu->GetHeight() / 3;

		snprintf( strBuf, 50, "Random playlist forced to sequential\nNext Index:%d", (int)m_uiPlaylistItem );

		// Draw the play position and subtitles
		DrawTextOnScreen( strBuf, iPosX, iPosY, DrawStyle_Text );
	}
}

bool DemoMusicPlaylistCallbacks::Init()
{
	DemoMusicCallbacksBase::Init();

	m_iPlayingID = AK::SoundEngine::PostEvent( 
		AK::EVENTS::PLAYMUSICDEMO2,
		GAME_OBJECT_MUSIC,
		AK_MusicPlaylistSelect | AK_EndOfEvent,
		&DemoMusicPlaylistCallbacks::MusicCallback,
		this
		);


	return true;
}

void DemoMusicPlaylistCallbacks::MusicCallback( AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo )
{
	DemoMusicPlaylistCallbacks* pPage = (DemoMusicPlaylistCallbacks*)in_pCallbackInfo->pCookie;
	
	if ( in_eType == AK_MusicPlaylistSelect )
	{
		AkMusicPlaylistCallbackInfo* pPlaylistInfo = static_cast<AkMusicPlaylistCallbackInfo*>( in_pCallbackInfo );
		pPlaylistInfo->uPlaylistItemDone = pPage->m_bStopPlaylist;
		pPlaylistInfo->uPlaylistSelection = pPage->m_uiPlaylistItem++;
		if ( pPage->m_uiPlaylistItem == pPlaylistInfo->uNumPlaylistItems )
			pPage->m_uiPlaylistItem = 0;
	}
	else if ( in_eType == AK_EndOfEvent )
	{
		pPage->m_bIsPlaying = false;
		pPage->m_uiPlaylistItem = 0;
	}
}

// *********************************************************************************
// MIDI Callbacks
// *********************************************************************************
DemoMIDICallbacks::DemoMIDICallbacks( Menu& in_ParentMenu )
: DemoMusicCallbacksBase( in_ParentMenu, "MIDI Callback Demo" )
, m_byNote( 0 )
, m_byVelocity( 0 )
, m_byCc( 0 )
, m_byValue( 0 )
{
	m_szHelp  = "Shows MIDI messages the game can receive using callbacks. MIDI messages"
				" include the MIDI notes, CC values, Pitch Bend, After Touch and Program Changes.";
}


void DemoMIDICallbacks::Draw()
{
	DemoMusicCallbacksBase::Draw();

	if ( m_bIsPlaying )
	{
		char strBuf[50];
		int iPosX = m_pParentMenu->GetWidth() / 4;
		int iPosY = m_pParentMenu->GetHeight() / 3;

		char strNote[10] = {0};
		MidiNoteToString( m_byNote, strNote, sizeof(strNote)/sizeof(char) );

		snprintf( strBuf, 50, "Last Note ON:%s\nVelocity:%d", strNote, m_byVelocity );

		// Draw the play position and subtitles
		DrawTextOnScreen( strBuf, iPosX, iPosY, DrawStyle_Text );

		iPosY = (int)(m_pParentMenu->GetHeight() *0.666);
		snprintf( strBuf, 50, "Last CC%d:%d", m_byCc, m_byValue );

		// Draw the play position and subtitles
		DrawTextOnScreen( strBuf, iPosX, iPosY, DrawStyle_Text );
	}
}

bool DemoMIDICallbacks::Init()
{
	DemoMusicCallbacksBase::Init();

	m_iPlayingID = AK::SoundEngine::PostEvent( 
		AK::EVENTS::PLAYMUSICDEMO3,
		GAME_OBJECT_MUSIC,
		AK_MIDIEvent | AK_EndOfEvent,
		&DemoMIDICallbacks::MusicCallback,
		this
		);


	return true;
}

void DemoMIDICallbacks::MusicCallback( AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo )
{
	DemoMIDICallbacks* pPage = (DemoMIDICallbacks*)in_pCallbackInfo->pCookie;
	
	if ( in_eType == AK_MIDIEvent )
	{
		AkMIDIEventCallbackInfo* pMidiInfo = static_cast<AkMIDIEventCallbackInfo*>( in_pCallbackInfo );

		if( pMidiInfo->midiEvent.byType == AK_MIDI_EVENT_TYPE_CONTROLLER )
		{
			pPage->m_byCc = pMidiInfo->midiEvent.Cc.byCc;
			pPage->m_byValue = pMidiInfo->midiEvent.Cc.byValue;
		}
		else if( pMidiInfo->midiEvent.byType == AK_MIDI_EVENT_TYPE_NOTE_ON )
		{
			pPage->m_byNote = pMidiInfo->midiEvent.NoteOnOff.byNote;
			pPage->m_byVelocity = pMidiInfo->midiEvent.NoteOnOff.byVelocity;
		}
	}
	else if ( in_eType == AK_EndOfEvent )
	{
		pPage->m_bIsPlaying = false;
	}
}
