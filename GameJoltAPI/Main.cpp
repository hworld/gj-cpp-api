#include "StdAfx.h"
#include "GameJoltAPI.h"

int main( void )
{

	GameJoltAPI api( 937, _T("9a8a5265bdf0803ebbd25a9ccb9459a1") );

	api.SetUsername( _T("") );
	api.SetUserToken( _T("") );

	if ( api.AuthUser() )
	{

		Trophy trophy = api.GetTrophy( 38 );

		wcout << trophy.GetTitle() << endl;

		wcout << _T("Achieved: ");

		if ( trophy.IsAchieved() )
			wcout << _T("Yes!") << endl;
		else
			wcout << _T("No!") << endl;

		wcout << _T("Achieved On: ") + trophy.GetAchievedDate() << endl;
		wcout << endl;

		DataStoreItem gameItem = api.GetDataStoreItem( DataStoreItem::GAME, _T("test") );
		DataStoreItem testItem = api.GetDataStoreItem( DataStoreItem::GAME, _T("test2") );
		DataStoreItem userItem = api.GetDataStoreItem( DataStoreItem::USER, _T("test") );

		wcout << _T("Game Data: ") + gameItem.GetData() << endl;
		wcout << _T("User Data: ") + userItem.GetData() << endl;

		api.SetDataStoreItem( userItem, _T("TestTest") );
		api.SetDataStoreItem( testItem, _T("Yarharhar") );

		wcout << _T("User Data: ") + userItem.GetData() << endl;

		api.RemoveDataStoreItem( userItem );

		vector<CStdString> keys = api.GetDataStoreKeys( DataStoreItem::GAME );

		api.AddScore( _T("Died 300 times"), 300 );
		api.AddScore( _T("Died 200 times"), 200, -1, _T("thisisextra"), _T("PoopyFace") );

		api.AddScore( _T("Died 300 times"), 300, 8571 );
		api.AddScore( _T("Died 200 times"), 200, 8571, _T("thisisextra"), _T("PoopyFace") );

		wcout << _T("-- Primary Table Scores --") << endl;
		vector<Score> scores = api.GetScores();
		vector<Score>::iterator it;

		for ( it = scores.begin(); it != scores.end(); it++ )
		{
			wcout << _T("Score: ") + it->GetScore() << endl;
		}

		wcout << _T("-- Secondary Table Scores --") << endl;
		vector<Score> scores2 = api.GetScores( 8571 );
		vector<Score>::iterator it2;

		for ( it2 = scores2.begin(); it2 != scores2.end(); it2++ )
		{
			wcout << _T("Score: ") + it2->GetScore() << endl;
		}

	}

	return 0;

}