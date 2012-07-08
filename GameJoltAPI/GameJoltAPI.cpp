#include "StdAfx.h"
#include "MD5.h"
#include "GameJoltAPI.h"
#include "Trophy.h"

GameJoltAPI::GameJoltAPI( int gameID /* = 0 */, CStdString gamePrivateKey /* = "" */ ): 
	GJAPI_SERVER( _T("gamejolt.com") ),
	GJAPI_ROOT( _T("/api/game/") ),
	GJAPI_VERSION( _T("v1") ),
	m_TrophiesPopulated( false ),
	m_GameID( gameID ),
	m_GamePrivateKey( gamePrivateKey )
{

	/* Nothing */

}


int GameJoltAPI::GetGameID()
{

	// Return the game ID.
	return m_GameID;

}


bool GameJoltAPI::SetGameID( int gameID )
{

	// Set the game ID.
	m_GameID = gameID;

	// Success!
	return true;

}


CStdString GameJoltAPI::GetGamePrivateKey()
{

	// Return the game's private key.
	return m_GamePrivateKey;

}


bool GameJoltAPI::SetGamePrivateKey( CStdString gamePrivateKey )
{

	// Set the game ID.
	m_GamePrivateKey = gamePrivateKey;

	// Success!
	return true;

}


CStdString GameJoltAPI::GetUsername()
{

	// Return the username.
	return m_Username;

}


bool GameJoltAPI::SetUsername( CStdString username )
{

	// Set the username.
	m_Username = username;

	// Return that there were no errors.
	return true;

}


CStdString GameJoltAPI::GetUserToken()
{

	// Return the user token.
	return m_UserToken;

}


bool GameJoltAPI::SetUserToken( CStdString userToken )
{

	// Set the user token.
	m_UserToken = userToken;

	// Return that there were no errors.
	return true;

}


void GameJoltAPI::AddErrorMessage( CStdString message )
{

	m_ErrorMessage += message;

}



CStdString GameJoltAPI::GetErrorMessage()
{

	// Return the error message string.
	return m_ErrorMessage;

}


void GameJoltAPI::ClearErrorMessages()
{

	m_ErrorMessage = _T("");

}


bool GameJoltAPI::AuthUser()
{

	m_ErrorMessage = _T("");

	bool ret = true;
	CStdString output;
	CStdString gameIDString;
	vector<map<CStdString, CStdString>> parsedOutput;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return true;
	}

	// Username must be set.
	if ( m_Username == _T("") )
	{
		m_ErrorMessage += _T("(The username must be set.)");
		return false;
	}

	// User token must be set.
	if ( m_UserToken == _T("") )
	{
		m_ErrorMessage += _T("(The user token must be set.)");
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );

	ret = SendRequest( output, _T("/users/auth/?game_id=") + gameIDString + _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken ) );

	if ( !ret )
	{
		return false;
		m_ErrorMessage += _T("(Could not authenticate the user.)");
	}

	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not authenticate the user.)");
		return false;
	}

	if ( parsedOutput.front()["success"] == _T("true") )
		return true;

	return false;

}


map<int, Trophy> GameJoltAPI::GetTrophies()
{

	if ( !m_TrophiesPopulated )
		PopulateTrophyInfo();

	return m_Trophies;

}


Trophy GameJoltAPI::GetTrophy( int trophyID )
{

	if ( !m_TrophiesPopulated )
		PopulateTrophyInfo();

	if ( m_Trophies.find( trophyID ) == m_Trophies.end() )
		return Trophy();

	return m_Trophies[trophyID];

}


bool GameJoltAPI::AddAchievedTrophy( int trophyID )
{

	m_ErrorMessage = _T("");

	bool ret;
	CStdString output;
	CStdString gameIDString;
	CStdString trophyIDString;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return false;
	}

	// Username must be set.
	if ( m_Username == _T("") )
	{
		m_ErrorMessage += _T("(The username must be set.)");
		return false;
	}

	// User token must be set.
	if ( m_UserToken == _T("") )
	{
		m_ErrorMessage += _T("(The user token must be set.)");
		return false;
	}

	// Trophy ID must be set.
	if ( trophyID <= 0 )
	{
		m_ErrorMessage += _T("(The trophy ID is wrong.)");
		return false;
	}

	// Format ints to strings.
	gameIDString.Format( _T("%d"), m_GameID );
	trophyIDString.Format( _T("%d"), trophyID );

	ret = SendRequest( output, _T("/trophies/add-achieved/?game_id=") + gameIDString + _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken ) + _T("&trophy_id=") + trophyIDString );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not add the trophy for the user.)");
		return false;
	}

	return true;

}


bool GameJoltAPI::AddAchievedTrophy( Trophy trophy )
{

	return AddAchievedTrophy( trophy.GetID() );

}


bool GameJoltAPI::PopulateTrophyInfo()
{

	CStdString output;
	CStdString gameIDString;
	vector<map<CStdString, CStdString>> parsedOutput;
	vector<map<CStdString, CStdString>>::iterator it;
	map<CStdString, CStdString> trophyInfo;
	bool ret = true;
	int trophyID = 0;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return false;
	}

	// Username must be set.
	if ( m_Username == _T("") )
	{
		m_ErrorMessage += _T("(The username must be set.)");
		return false;
	}

	// User token must be set.
	if ( m_UserToken == _T("") )
	{
		m_ErrorMessage += _T("(The user token must be set.)");
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );

	// Send the request.
	ret = SendRequest( output, _T("/trophies/?game_id=") + gameIDString + _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken ) );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not populate the trophy information.)");
		return false;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not populate the trophy information.)");
		return false;
	}

	for ( it = parsedOutput.begin(); it != parsedOutput.end(); it++ )
	{
		trophyInfo = (*it);
		trophyID = atoi( CStdStringA( trophyInfo["id"] ) );
		m_Trophies[trophyID] = Trophy();
		m_Trophies[trophyID].Initialize( this, trophyInfo );
	}

	m_TrophiesPopulated = true;

	return true;

}


DataStoreItem GameJoltAPI::GetDataStoreItem( DataStoreItem::Type type, CStdString key )
{

	DataStoreItem dataItem;

	dataItem.Initialize( this, type, key );

	return dataItem;

}


vector<CStdString> GameJoltAPI::GetDataStoreKeys( DataStoreItem::Type type )
{

	CStdString output;
	CStdString gameIDString;
	CStdString requestURL;
	vector<map<CStdString, CStdString>> parsedOutput;
	vector<map<CStdString, CStdString>>::iterator it;
	vector<CStdString> dataKeys;
	bool ret = true;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return dataKeys;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );

	requestURL = _T("/data-store/get-keys/?game_id=") + gameIDString;

	if ( type == DataStoreItem::USER )
	{

		// Username must be set.
		if ( m_Username == _T("") )
		{
			m_ErrorMessage += _T("(The username must be set.)");
			return dataKeys;
		}

		// User token must be set.
		if ( m_UserToken == _T("") )
		{
			m_ErrorMessage += _T("(The user token must be set.)");
			return dataKeys;
		}

		requestURL += _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken );

	}

	// Send the request.
	ret = SendRequest( output, requestURL );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not get the data storage keys.)");
		return dataKeys;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not get the data storage keys.)");
		return dataKeys;
	}

	for ( it = parsedOutput.begin(); it != parsedOutput.end(); it++ )
	{
		dataKeys.push_back( (*it)["key"] );
	}

	return dataKeys;

}


bool GameJoltAPI::SetDataStoreItem( DataStoreItem item, CStdString data )
{

	CStdString output;
	CStdString gameIDString;
	CStdString requestURL;
	vector<map<CStdString, CStdString>> parsedOutput;
	bool ret = true;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );

	requestURL = _T("/data-store/set/?game_id=") + gameIDString + _T("&key=") + EncodeURL( item.GetKey() ) + _T("&data=") + EncodeURL( data );

	if ( item.GetType() == DataStoreItem::USER )
	{

		// Username must be set.
		if ( m_Username == _T("") )
		{
			m_ErrorMessage += _T("(The username must be set.)");
			return false;
		}

		// User token must be set.
		if ( m_UserToken == _T("") )
		{
			m_ErrorMessage += _T("(The user token must be set.)");
			return false;
		}

		requestURL += _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken );

	}

	// Send the request.
	ret = SendRequest( output, requestURL );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not set the data item.)");
		return false;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not set the data item.)");
		return false;
	}

	if ( parsedOutput.front()["success"] == _T("false") )
	{
		m_ErrorMessage += parsedOutput.front()["message"];
		return false;
	}

	return true;

}


bool GameJoltAPI::RemoveDataStoreItem( DataStoreItem item )
{

	CStdString output;
	CStdString gameIDString;
	CStdString requestURL;
	vector<map<CStdString, CStdString>> parsedOutput;
	bool ret = true;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );

	requestURL = _T("/data-store/remove/?game_id=") + gameIDString + _T("&key=") + EncodeURL( item.GetKey() );

	if ( item.GetType() == DataStoreItem::USER )
	{

		// Username must be set.
		if ( m_Username == _T("") )
		{
			m_ErrorMessage += _T("(The username must be set.)");
			return false;
		}

		// User token must be set.
		if ( m_UserToken == _T("") )
		{
			m_ErrorMessage += _T("(The user token must be set.)");
			return false;
		}

		requestURL += _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken );

	}

	// Send the request.
	ret = SendRequest( output, requestURL );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not remove the data item.)");
		return false;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not remove the data item.)");
		return false;
	}

	if ( parsedOutput.front()["success"] == _T("false") )
	{
		m_ErrorMessage += parsedOutput.front()["message"];
		return false;
	}

	return true;

}


bool GameJoltAPI::AddScore( CStdString score, int sort, int tableId /* = 0 */, CStdString extraData /* = _T("") */, CStdString guest /* = _T("") */ )
{

	CStdString output;
	CStdString gameIDString;
	CStdString tableIdString;
	CStdString sortString;
	CStdString requestURL;
	vector<map<CStdString, CStdString>> parsedOutput;
	bool ret = true;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return false;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );
	sortString.Format( _T("%d"), sort );
	tableIdString.Format( _T("%d"), tableId );

	requestURL = _T("/scores/add/?game_id=") + gameIDString + _T("&score=") + EncodeURL( score ) + _T("&sort=") + sortString + _T("&extra_data=") + EncodeURL( extraData );

	if ( guest == _T("") )
	{
		// Username must be set.
		if ( m_Username == _T("") )
		{
			m_ErrorMessage += _T("(The username must be set.)");
			return false;
		}

		// User token must be set.
		if ( m_UserToken == _T("") )
		{
			m_ErrorMessage += _T("(The user token must be set.)");
			return false;
		}

		requestURL += _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken );
	}
	else
	{
		requestURL += _T("&guest=") + EncodeURL( guest );
	}

	// Did they pass in a table ID?
	if ( tableId > 0 ) {
		requestURL += _T("&table_id=") + tableIdString;
	}

	// Send the request.
	ret = SendRequest( output, requestURL );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not add the score.)");
		return false;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not add the score.)");
		return false;
	}

	if ( parsedOutput.front()["success"] == _T("false") )
	{
		m_ErrorMessage += parsedOutput.front()["message"];
		return false;
	}

	return true;

}


vector<Score> GameJoltAPI::GetScores( int tableId /* = 0 */, bool user /* = false */, int limit /* = -1 */ )
{

	CStdString output;
	CStdString gameIDString;
	CStdString tableIdString;
	CStdString limitString;
	CStdString requestURL;
	vector<map<CStdString, CStdString>> parsedOutput;
	vector<map<CStdString, CStdString>>::iterator it;
	map<CStdString, CStdString> scoreInfo;
	vector<Score> scores;
	bool ret = true;

	// Game ID must be set.
	if ( m_GameID <= 0 )
	{
		m_ErrorMessage += _T("(The game ID must be set.)");
		return scores;
	}

	// We must format the Game ID to a string.
	gameIDString.Format( _T("%d"), m_GameID );
	tableIdString.Format( _T("%d"), tableId );
	limitString.Format( _T("%d"), limit );

	requestURL = _T("/scores/?game_id=") + gameIDString;

	if ( user )
	{
		// Username must be set.
		if ( m_Username == _T("") )
		{
			m_ErrorMessage += _T("(The username must be set.)");
			return scores;
		}

		// User token must be set.
		if ( m_UserToken == _T("") )
		{
			m_ErrorMessage += _T("(The user token must be set.)");
			return scores;
		}

		requestURL += _T("&username=") + EncodeURL( m_Username ) + _T("&user_token=") + EncodeURL( m_UserToken );
	}

	if ( limit > 0 ) {
		requestURL += _T("&limit=") + limitString;
	}

	if ( tableId > 0 ) {
		requestURL += _T("&table_id=") + tableIdString;
	}

	// Send the request.
	ret = SendRequest( output, requestURL );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not get the scores.)");
		return scores;
	}

	// Parse the output.
	ret = ParseOutputString( output, parsedOutput );

	if ( !ret )
	{
		m_ErrorMessage += _T("(Could not get the scores.)");
		return scores;
	}

	Score score;

	for ( it = parsedOutput.begin(); it != parsedOutput.end(); it++ )
	{
		scoreInfo = (*it);
		score = Score();
		score.Initialize( this, scoreInfo );
		scores.push_back( score );
	}

	return scores;

}



bool GameJoltAPI::SendRequest( CStdString &output, CStdString url )
{

	// The private key must be set to send a request.
	if ( m_GamePrivateKey == _T("") )
	{
		m_ErrorMessage += _T("(You must put in your game's private key before you can use any of the API functions.)");
		return false;
	}

	////////////////////////////////////
	// Let's form the URL first.

	url = GJAPI_ROOT + GJAPI_VERSION + url;
	CStdString signature( md5( CStdStringA( _T("http://") + GJAPI_SERVER + url + m_GamePrivateKey ) ) );
	url += _T("&signature=") + signature;

	// Now let's build the request.
	BOOL ret = FALSE;
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;

	hSession = WinHttpOpen
	(
		L"Game Jolt API Construct/1.0", 
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 
		0
	);

	if ( hSession )
		hConnect = WinHttpConnect
		( 
			hSession, 
			CStdStringW( GJAPI_SERVER ),
			INTERNET_DEFAULT_PORT, 
			0
		);
	else
		m_ErrorMessage += _T("(Could not open HTTP session.)");

	if ( hConnect )
		hRequest = WinHttpOpenRequest
		( 
			hConnect,
			L"GET",
			CStdStringW( url ),
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_REFRESH
		);
	else
		m_ErrorMessage += _T("(Could not connect to the HTTP session.)");

	if ( hRequest ) 
		ret = WinHttpSendRequest
		( 
			hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			WINHTTP_NO_REQUEST_DATA,
			0,
			0,
			0
		);
	else
		m_ErrorMessage += _T("(Could not set up the HTTP request.)");

	if ( ret )
		ret = WinHttpReceiveResponse( hRequest, NULL );
	else
		m_ErrorMessage += _T("(Could not send the HTTP request.)");

	DWORD bufferSize = 0;
	DWORD outputDownloaded = 0;
	LPSTR outputBuffer = 0;

	// Keep checking for data until there is nothing left.
	if ( ret )
	{
		do 
		{

			// Check for available data.
			bufferSize = 0;
			WinHttpQueryDataAvailable( hRequest, &bufferSize );

			// Allocate space for the buffer.
			outputBuffer = new char[bufferSize + 1];
			if ( outputBuffer )
			{
				// Read the data.
				ZeroMemory( outputBuffer, bufferSize + 1 );

				if ( WinHttpReadData( hRequest, (LPVOID)outputBuffer, bufferSize, &outputDownloaded ) )
					output += outputBuffer;

				// Free the memory allocated to the buffer.
				delete [] outputBuffer;
			}

		} while( bufferSize > 0 );
	}
	else
		m_ErrorMessage += _T("(Did not get a response from the server.)");

	if ( hRequest ) WinHttpCloseHandle( hRequest );
    if ( hConnect ) WinHttpCloseHandle( hConnect );
    if ( hSession ) WinHttpCloseHandle( hSession );

	return true;

}


bool GameJoltAPI::ParseOutputString( CStdString inputString, vector<map<CStdString, CStdString>> &returnData )
{

	map<CStdString, CStdString> resourceData;
	vector<CStdString> parsedLines;
	vector<CStdString>::iterator it;
	CStdString curPiece;
	CStdString curKey;
	CStdString curValue;
	int curPos = 0;

	curPiece = inputString.Tokenize( _T("\n"), curPos );
	while ( curPiece != _T("") )
	{
		parsedLines.push_back( curPiece );
		curPiece = inputString.Tokenize( _T("\n"), curPos );
	}

	if ( parsedLines.size() == 0 )
	{
		m_ErrorMessage += _T("(Could not parse the response.)");
		return false;
	}

	for ( it = parsedLines.begin(); it < parsedLines.end(); it++ )
	{

		curPos = (*it).FindOneOf( _T(":") );
		curKey = (*it).Mid( 0, curPos );
		curValue = (*it).Mid( curPos + 2, (*it).GetLength() - curPos - 4 );
		
		// If this key is set, then we have looped and have a new resource.
		// Push the data and clear.
		if ( resourceData[curKey] != _T("") )
		{
			returnData.push_back( resourceData );
			resourceData.clear();
		}

		resourceData[curKey] = curValue;

	}

	// We do one final push.
	returnData.push_back( resourceData );

	if ( returnData.front()["success"] != _T("true") )
		m_ErrorMessage += _T("(") + returnData.front()["message"] + _T(")");

	// Return it.
	return true;

}


CStdString GameJoltAPI::EncodeURL( CStdString inputString )
{

	CStdString escapedString = "";

	int max = inputString.length();
	for ( int i = 0; i < max; i++ )
	{

		if 
		( 
			(48 <= inputString[i] && inputString[i] <= 57) || //0-9
			(65 <= inputString[i] && inputString[i] <= 90) || //abc...xyz
			(97 <= inputString[i] && inputString[i] <= 122) || //ABC...XYZ
			(
				inputString[i] == '~' || inputString[i] == '!' || 
				inputString[i]=='*' || inputString[i] == '(' || 
				inputString[i] == ')' || inputString[i] == '\''
			)
		)
		{
			escapedString += inputString[i];
		}
		else
		{
			escapedString += _T("%");
			escapedString += Char2Hex( inputString[i] ); //converts char 255 to string "ff"
		}

	}

	return escapedString;

}
	

CStdString GameJoltAPI::Char2Hex( TCHAR character )
{

	char dig1 = (character&0xF0)>>4;
    char dig2 = (character&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    CStdString r;
    r += dig1;
    r += dig2;
    return r;

}