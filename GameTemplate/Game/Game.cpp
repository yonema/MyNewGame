#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"

/**
 * @brief ���̃Q�[���̃l�[���X�y�[�X
 * @todo MyGame�������ƃ��j�[�N�ȃQ�[���̃^�C�g�����Ƃ��ɕύX����
*/
namespace nsMyGame
{
	/**
	 * @brief �Q�[���t�@�C���̃l�[���X�y�[�X
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