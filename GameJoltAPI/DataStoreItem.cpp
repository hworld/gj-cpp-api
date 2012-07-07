#include "StdAfx.h"
#include "GameJoltAPI.h"
#include "DataStoreItem.h"


DataStoreItem::DataStoreItem():
	m_Initialized( false ),
	m_Type( DataStoreItem::GAME ),
	m_Key( _T("") ),
	m_Data( _T("") )
{

	/* Nothing */

}


bool DataStoreItem::Initialize( GameJoltAPI* api, DataStoreItem::Type type, const CStdString& key )
{

	m_pAPI = api;

	m_Type = type;
	m_Key = key;

	m_Initialized = true;

	return true;

}


DataStoreItem::Type DataStoreItem::GetType() const
{

	return m_Type;

}


CStdString DataStoreItem::GetKey() const
{

	return m_Key;

}


CStdString DataStoreItem::GetData()
{

	CStdString output;
	CStdString gameIDString;
	CStdString requestURL;
	std::vector< std::map< CStdString, CStdString > > parsedOutput;
	std::map< CStdString, CStdString > itemData;
	DataStoreItem dataItem;
	bool ret = true;

	// Game ID must be set.
	if ( m_pAPI->GetGameID() <= 0 )
	{
		m_pAPI->AddErrorMessage( _T("(The game ID must be set.)") );
		return _T("");
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_pAPI->GetGameID() );

	requestURL = _T("/data-store/?game_id=" + gameIDString) + _T("&key=") + m_pAPI->EncodeURL( m_Key );

	// Is this a user item?
	if ( m_Type == DataStoreItem::USER )
	{

		// Username must be set.
		if ( m_pAPI->GetUsername() == _T("") )
		{
			m_pAPI->AddErrorMessage( _T("(The username must be set.)") );
			return _T("");
		}

		// User token must be set.
		if ( m_pAPI->GetUserToken() == _T("") )
		{
			m_pAPI->AddErrorMessage( _T("(The user token must be set.)") );
			return _T("");
		}

		requestURL += _T("&username=") + m_pAPI->EncodeURL( m_pAPI->GetUsername() ) + _T("&user_token=") + m_pAPI->EncodeURL( m_pAPI->GetUserToken() );

	}

	// Send the request.
	ret = m_pAPI->SendRequest( output, requestURL );

	if ( !ret )
	{
		m_pAPI->AddErrorMessage( _T("(Could not send the request for getting the stored item.)") );
		return _T("");
	}

	// Parse the output.
	ret = m_pAPI->ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_pAPI->AddErrorMessage( _T("(Could not get data for the key passed in.)") );
		return _T("");
	}

	itemData = parsedOutput.front();

	if ( itemData["success"] != _T("true") )
	{
		m_pAPI->AddErrorMessage( _T("(Could not get data for the key passed in.)") );
		return _T("");
	}

	m_Data = itemData["data"];

	return m_Data;

}