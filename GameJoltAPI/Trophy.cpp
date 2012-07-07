#include "StdAfx.h"
#include "Trophy.h"
#include "GameJoltAPI.h"


Trophy::Trophy():
	m_Initialized( false ),
	m_ID( 0 ),
	m_Title( "" ),
	m_Description( "" ),
	m_Difficulty( "" ),
	m_ImageURL( "" )
{

	/* Nothing */

}


bool Trophy::Initialize( GameJoltAPI *api, std::map< CStdString, CStdString >& trophyInfo )
{

	m_pAPI = api;
	
	m_ID = atoi( CStdStringA( trophyInfo["id"] ) );
	m_Title = trophyInfo["title"];
	m_Description = trophyInfo["description"];
	m_Difficulty = trophyInfo["difficulty"];
	m_ImageURL = trophyInfo["image_url"];

	m_Initialized = true;

	return true;
	
}


int Trophy::GetID() const
{

	return m_ID;

}


CStdString Trophy::GetTitle() const
{

	return m_Title;

}


CStdString Trophy::GetDescription() const
{

	return m_Description;

}


CStdString Trophy::GetDifficulty() const
{

	return m_Difficulty;

}


CStdString Trophy::GetImageURL() const
{

	return m_ImageURL;

}


CStdString Trophy::GetAchievedDate()
{

	bool ret;
	std::map< CStdString, CStdString > trophyInfo;

	ret = GetTrophyInfo( trophyInfo );

	if ( !ret )
		return "";

	if ( trophyInfo["achieved"] == _T("false") )
		return "";

	return trophyInfo["achieved"];

}


bool Trophy::IsAchieved()
{

	bool ret;
	std::map< CStdString, CStdString > trophyInfo;

	ret = GetTrophyInfo( trophyInfo );

	if ( !ret )
		return false;

	if ( trophyInfo["achieved"] != _T("false") )
		return true;

	return false;

}


bool Trophy::GetTrophyInfo( std::map< CStdString, CStdString >& trophyInfo )
{

	CStdString output;
	CStdString gameIDString;
	CStdString trophyIDString;
	std::vector< std::map< CStdString, CStdString > > parsedOutput;
	bool ret = true;
	int trophyID = 0;

	// Game ID must be set.
	if ( m_pAPI->GetGameID() <= 0 )
	{
		m_pAPI->AddErrorMessage( _T("(The game ID must be set.)") );
		return false;
	}

	// Username must be set.
	if ( m_pAPI->GetUsername() == _T("") )
	{
		m_pAPI->AddErrorMessage( _T("(The username must be set.)") );
		return false;
	}

	// User token must be set.
	if ( m_pAPI->GetUserToken() == _T("") )
	{
		m_pAPI->AddErrorMessage( _T("(The user token must be set.)") );
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_pAPI->GetGameID() );
	trophyIDString.Format( _T("%d"), m_ID );

	// Send the request.
	ret = m_pAPI->SendRequest( output, _T("/trophies/?game_id=") + gameIDString + _T("&username=") + m_pAPI->EncodeURL( m_pAPI->GetUsername() ) + _T("&user_token=") + m_pAPI->EncodeURL(  m_pAPI->GetUserToken() ) + _T("&trophy_id=") + trophyIDString );

	if ( !ret )
	{
		m_pAPI->AddErrorMessage( _T("(Could not get whether the trophy has been achieved or not.)") );
		return false;
	}

	// Parse the output.
	ret = m_pAPI->ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_pAPI->AddErrorMessage( _T("(Could not get whether the trophy has been achieved or not.)") );
		return false;
	}

	trophyInfo = parsedOutput.front();

	return true;

}