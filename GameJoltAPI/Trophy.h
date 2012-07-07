#ifndef TROPHY_H
#define TROPHY_H

class GameJoltAPI;

class Trophy
{

public:

	Trophy();

	bool Initialize( GameJoltAPI* api, std::map< CStdString, CStdString >& trophyInfo );

	int GetID() const;
	CStdString GetTitle() const;
	CStdString GetDescription() const;
	CStdString GetDifficulty() const;
	CStdString GetAchievedDate();
	CStdString GetImageURL() const;
	bool IsAchieved();

private:

	// Utility functions.
	bool GetTrophyInfo( std::map< CStdString, CStdString >& trophyInfo );

	bool m_Initialized;

	GameJoltAPI *m_pAPI;

	int m_ID;
	CStdString m_Title;
	CStdString m_Description;
	CStdString m_Difficulty;
	CStdString m_ImageURL;

};

#endif