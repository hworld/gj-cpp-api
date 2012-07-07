#ifndef DATASTOREITEM_H
#define DATASTOREITEM_H

class GameJoltAPI;

class DataStoreItem
{

public:

	enum Type
	{
		GAME,
		USER
	};

	DataStoreItem();

	bool Initialize( GameJoltAPI* api, DataStoreItem::Type type, const CStdString& key );

	DataStoreItem::Type GetType() const;
	CStdString GetKey() const;
	CStdString GetData();

private:

	bool m_Initialized;

	GameJoltAPI *m_pAPI;

	DataStoreItem::Type m_Type;
	CStdString m_Key;
	CStdString m_Data;

};

#endif