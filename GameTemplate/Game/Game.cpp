#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"

/**
 * @brief このゲームのネームスペース
 * @todo MyGameをもっとユニークなゲームのタイトル名とかに変更する
*/
namespace nsMyGame
{
	/**
	 * @brief ゲームファイルのネームスペース
	*/
	namespace nsGame
	{
		bool CGame::Start()
		{
			NewGO<nsMaps::CTestMap>(nsCommonData::enPriorityFirst);

			return true;
		}
	}
}