/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"
#include "../../GameTemplate/Game/GameTime.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

//追加
//ゲームがポーズ中か？　初期値はポーズ中ではない
bool GameObjectManager::m_isGamePaused = false;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//死亡フラグがついているゲームオブジェクトを破棄する。
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}

	//追加


		//ポーズ中ではない
	for (auto& goList : m_gameObjectListArray)
	{
		for (auto& go : goList)
		{
			if (m_isGamePaused != true)
			{
				go->UpdateWrapper();
			}
			else
			{
				//ポーズ中に呼ばれるアップデート処理
				go->UpdateOnlyPausedWrapper();
			}
			//ポーズ中でもポーズ中でなくても関係なく常に呼ばれるアップデート処理
			go->AlwaysUpdateWrapper();
		}
	}
	


	//物理エンジンのアップデートを呼び出す。
	PhysicsWorld::GetInstance()->Update(nsNinjaAttract::nsTimer::GameTime().GetFrameDeltaTime());
}
//void GameObjectManager::ExecuteRender(RenderContext& rc)
//{
//	//レンダラーを変更するならここを改造していくと良い。
//	for (auto& goList : m_gameObjectListArray) {
//		for (auto& go : goList) {
//			go->RenderWrapper(rc);
//		}
//	}
//}


// 追加

/**
 * @brief 描画オブジェクトの登録を実行する。
*/
void GameObjectManager::ExecuteAddRender()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->AddRenderEntranceWrapper();
		}
	}
}