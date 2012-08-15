#ifndef _CSlasherGameManager_h
#define _CSlasherGameManager_h

#include "CGameManager.h"
#include "TSingleton.h"

class CSlasherGameManager :  public GD4N::CGameManager {
	friend class GD4N::TSingleton<CSlasherGameManager>;
protected:
	CSlasherGameManager();
	bool Init();
	void LoadResources();

	void Scene00();
};

#define sGameManager GD4N::TSingleton< CSlasherGameManager >::GetInstance()

#endif