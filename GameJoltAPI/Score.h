#ifndef SCORE_H
#define SCORE_H

class GameJoltAPI;

class Score
{

public:

	Score();

	bool Initialize( GameJoltAPI* api, std::map< CStdString, CStdString >& scoreInfo );

	CStdString GetScore() const;
	int GetSort() const;
	CStdString GetExtraData() const;
	CStdString GetUser() const;
	int GetUserID() const;
	CStdString GetGuest() const;
	CStdString GetStoredDate() const;

private:

	bool m_Initialized;

	GameJoltAPI* m_pAPI;

	CStdString m_Score;
	int m_Sort;
	CStdString m_ExtraData;
	CStdString m_User;
	int m_UserID;
	CStdString m_Guest;
	CStdString m_StoredDate;

};

#endif