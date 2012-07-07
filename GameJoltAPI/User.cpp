#include "StdAfx.h"
#include "User.h"


bool User::Initialize( GameJoltAPI* api )
{

	m_pAPI = api;

	return true;

}


CStdString User::GetUsername() const
{

	return m_Username;

}


bool User::SetUsername( const CStdString& username )
{

	m_Username = username;

	return true;

}


CStdString User::GetUserToken() const
{

	return m_UserToken;

}


bool User::SetUserToken( const CStdString& userToken )
{

	m_UserToken = userToken;

	return true;

}