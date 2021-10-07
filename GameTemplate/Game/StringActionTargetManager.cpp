#include "stdafx.h"
#include "StringActionTargetManager.h"
#include "SwingTarget.h"

namespace nsMyGame
{

	/**
	 * @brief �����g�����A�N�V�����̃^�[�Q�b�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsStringActionTarget
	{

		CStringActionTargetManager* CStringActionTargetManager::m_instance = nullptr;//!< �B��̃C���X�^���X		


		/**
		 * @brief �R���X�g���N�^
		*/
		CStringActionTargetManager::CStringActionTargetManager()
		{
			// �X�C���O�̃^�[�Q�b�g�̃R���e�i�̃L���p�V�e�B���m��
			m_swingTargets.reserve(m_kSwingTargetNum);

			return;
		}

		/**
		 * @brief �X�C���O�^�[�Q�b�g�̓o�^����������
		 * @param[in] swingTarget ��������X�C���O�^�[�Q�b�g
		*/
		void CStringActionTargetManager::RemoveSwingTarget(const nsSwingTarget::CSwingTarget* swingTarget)
		{
			// ��������X�C���O�^�[�Q�b�g��T��
			std::vector<nsSwingTarget::CSwingTarget*>::iterator itr
				= std::find(m_swingTargets.begin(), m_swingTargets.end(), swingTarget);

			// ��������
			m_swingTargets.erase(itr);

			return;
		}


		/**
		 * @brief �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W�𓾂�
		 * @param[in] fromPos ���W
		 * @param[in] scopeRadius �L���͈͂̔��a
		 * @return �X�C���O�^�[�Q�b�g�̍��W�B�L���͈͓��ɖ������nullptr��߂��B
		*/
		const Vector3* const GetNearestSwintTargetPointWithinScope(
			const Vector3& fromPos,
			const float scopeRadius
		)
		{
			Vector3 fromPosXZ = fromPos;
			fromPosXZ.y = 0.0f;

			float tmpDist = FLT_MAX;
			const Vector3* retPosition = nullptr;


			CStringActionTargetManager::GetInstance()->QuerySwingTarget(
				[&](nsSwingTarget::CSwingTarget* swingTarget)
				{
					Vector3 swingTargetPosXZ = swingTarget->GetPosition();
					swingTargetPosXZ.y = 0.0f;

					Vector3 distVec = fromPosXZ - swingTargetPosXZ;
					if (distVec.Length() > scopeRadius)
					{
						return;
					}

					const std::vector<Vector3>& stPositions = swingTarget->GetSwingTargetPositions();

					for (auto& stPos : stPositions)
					{

						distVec = fromPos - stPos;

						float destLen = distVec.Length();

						if (destLen < tmpDist)
						{
							tmpDist = destLen;
							retPosition = &stPos;
						}
					}


				}
			);

			return retPosition;
		}

	}
}