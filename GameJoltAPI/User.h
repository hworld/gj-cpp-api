#ifndef USER_H
#define USER_H

class GameJoltAPI;

class User
{

public:

	User();

	bool Initialize( GameJoltAPI* api );

	CStdString GetUsername() const;
	bool SetUsername( const CStdString& username );

	CStdString GetUserToken() const;
	bool SetUserToken( const CStdString& userToken );

private:

	bool m_Initialized;

	GameJoltAPI* m_pAPI;

	CStdString m_Username;
	CStdString m_UserToken;

};

#endif