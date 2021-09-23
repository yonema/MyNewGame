/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"
#include "../../GameTemplate/Game/GameTime.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

//�ǉ�
//�Q�[�����|�[�Y�����H�@�����l�̓|�[�Y���ł͂Ȃ�
bool GameObjectManager::m_isGamePaused = false;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
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
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
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

	//�ǉ�

	//�|�[�Y���ł͂Ȃ����H
	if (m_isGamePaused == false)
	{
		//�|�[�Y���ł͂Ȃ�
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->UpdateWrapper();
			}
		}
	}
	else
	{
		//�|�[�Y��
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				//�|�[�Y���ɌĂ΂��A�b�v�f�[�g����
				go->UpdateOnlyPausedWrapper();
			}
		}
	}

	//�|�[�Y���ł��|�[�Y���łȂ��Ă��֌W�Ȃ���ɌĂ΂��A�b�v�f�[�g����
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->AlwaysUpdateWrapper();
		}
	}

	//�����G���W���̃A�b�v�f�[�g���Ăяo���B
	PhysicsWorld::GetInstance()->Update(nsMyGame::nsTimer::GameTime().GetFrameDeltaTime());
}
//void GameObjectManager::ExecuteRender(RenderContext& rc)
//{
//	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
//	for (auto& goList : m_gameObjectListArray) {
//		for (auto& go : goList) {
//			go->RenderWrapper(rc);
//		}
//	}
//}


// �ǉ�

/**
 * @brief �`��I�u�W�F�N�g�̓o�^�����s����B
*/
void GameObjectManager::ExecuteAddRender()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->AddRenderEntranceWrapper();
		}
	}
}