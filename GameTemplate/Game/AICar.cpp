#include "stdafx.h"
#include "AICar.h"

namespace nsMyGame
{
	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief �h���N���X�p��Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CAICar::StartSub()
		{
			InitModel(
				"Assets/modelData/car/sedan_car.tkm", 
				{ -1000.0f,10.0f,0.0f },
				Quaternion::Identity, Vector3::One
			);

			return true;
		}

		/**
		 * @brief �h���N���X�p�̍X�V����
		*/
		void CAICar::UpdateSub()
		{
			//if (g_pad[0]->IsTrigger(enButtonX))
			//{
			//	PathFinding(g_camera3D->GetTarget());
			//}
			//return;

			if (IsEndPathMove() != true)
			{
				MoveOnPath(5000.0f, PhysicsWorld::GetInstance());
			}
			else
			{
				RandomTargetPathFinding();
			}
			return;
		}
	}
}