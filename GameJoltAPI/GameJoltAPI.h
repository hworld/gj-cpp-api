#ifndef GAMEJOLTAPI_H
#define GAMEJOLTAPI_H

using namespace std;

#include "Trophy.h"
#include "DataStoreItem.h"
#include "Score.h"

class GameJoltAPI
{

public:

	// Class Constants.
	const CStdString GJAPI_SERVER;
	const CStdString GJAPI_ROOT;
	const CStdString GJAPI_VERSION;
	
	// Constructor/destructor.
	GameJoltAPI( int gameID = 0, CStdString gamePrivateKey = "" );

	// Public functions.
	int GetGameID();
	bool SetGameID( int gameID );

	CStdString GetGamePrivateKey();
	bool SetGamePrivateKey( CStdString gamePrivateKey );

	CStdString GetUsername();
	bool SetUsername( CStdString username );

	CStdString GetUserToken();
	bool SetUserToken( CStdString userToken );

	void AddErrorMessage( CStdString message );  // Add the error message.
	CStdString GetErrorMessage();  // Error messages build up in the object. This is to retrieve any errors that may have gone wrong.
	void ClearErrorMessages(); // Clear all the error messages that were built up.

	bool AuthUser();  // Tries to authorizes the stored user. Returns whether or not the stored username/token are correct.

	map<int, Trophy> GetTrophies();  // Returns a map of all trophies for this game.
	//bool GetTrophies( map<int, Trophy*> &trophies, bool achieved );  // Returns a map of all trophies. Pass in whether you only want achieved or unachieved.
	Trophy GetTrophy( int trophyID );  // Returns the trophy with the ID passed in..
	bool AddAchievedTrophy( int trophyID );  // Adds an achieved trophy. Pass in the trophy ID.
	bool AddAchievedTrophy( Trophy trophy );  // Adds an achieved trophy. Pass in a trophy object.

	DataStoreItem GetDataStoreItem( DataStoreItem::Type type, CStdString key );
	//vector<DataStoreItem> GetDataStoreItems( DataStoreItem::Type type );
	vector<CStdString> GetDataStoreKeys( DataStoreItem::Type type );
	//bool SetDataStoreItem( CStdString key );
	bool SetDataStoreItem( DataStoreItem item, CStdString data );
	//bool RemoveDataStoreItem( CStdString key );
	bool RemoveDataStoreItem( DataStoreItem item );

	vector<Score> GetScores( int tableId = 0, bool user = false, int limit = -1 );
	bool AddScore( CStdString score, int sort, int tableId = 0, CStdString extraData = _T(""), CStdString guest = _T("") );

	// Utility functions.
	bool PopulateTrophyInfo();
	bool SendRequest( CStdString &output, CStdString url );
	bool ParseOutputString( CStdString inputString, vector<map<CStdString, CStdString>> &returnData );
	CStdString EncodeURL( CStdString inputString );
	CStdString Char2Hex( TCHAR character );


private:

	// Private variables.
	int m_GameID;
	CStdString m_GamePrivateKey;

	//User m_User;
	CStdString m_Username;
	CStdString m_UserToken;

	CStdString m_ErrorMessage;

	// Mapping between trophy IDs and trophy objects.
	map<int, Trophy> m_Trophies;
	bool m_TrophiesPopulated;

};

#endif