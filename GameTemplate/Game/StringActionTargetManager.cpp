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
			// Y���W�i�����j�𖳎�����XZ���ʂł̍��W
			Vector3 fromPosXZ = fromPos;
			fromPosXZ.y = 0.0f;

			// �ŒZ������ێ����Ă����ϐ�
			float tmpDist = FLT_MAX;

			// �߂�l�p�̍��W�̃|�C���^
			const Vector3* retPosition = nullptr;

			// �S�ẴX�C���O�^�[�Q�b�g�ɃN�G������
			CStringActionTargetManager::GetInstance()->QuerySwingTarget(
				[&](nsSwingTarget::CSwingTarget* swingTarget)
				{
					//////// 1.�X�C���O�^�[�Q�b�g��XZ���ʂŁA�L���͈͓������ׂ� ////////

					// �X�C���O�^�[�Q�b�g��Y���W�i�����j�𖳎�����XZ���ʂł̍��W
					Vector3 swingTargetPosXZ = swingTarget->GetPosition();
					swingTargetPosXZ.y = 0.0f;

					// XZ���ʂł̃X�C���O�^�[�Q�b�g�Ƃ̋����x�N�g��
					Vector3 distVec = fromPosXZ - swingTargetPosXZ;	// �����x�N�g��
					// �������l�����Ȃ��ŁA�X�C���O�^�[�Q�b�g���L���͈͓������ׂ�
					if (distVec.Length() > scopeRadius)
					{
						// �L���͈͓��ł͂Ȃ�
						// �������^�[��
						return;
					}

					//////// 2.�X�C���O�^�[�Q�b�g�̍��W�����̒��ň�ԋ߂����W��T�� ////////

					// �X�C���O�^�[�Q�b�g�̍��W�R���e�i���擾
					const std::vector<Vector3>& stPositions = swingTarget->GetSwingTargetPositions();

					// �S�Ă̍��W�𒲂ׂ�
					for (auto& stPos : stPositions)
					{
						// ���W�Ƃ̋����x�N�g��
						distVec = fromPos - stPos;

						// ���W�Ƃ̋���
						const float destLen = distVec.Length();

						// ����̋��������܂ł̍ŒZ�������Z�����H
						if (destLen < tmpDist)
						{
							// �Z���ꍇ�͂��̋����Ƃ��̍��W��ێ�
							tmpDist = destLen;
							retPosition = &stPos;
						}
					}

					// �N�G���I��
					return;
				}
			);

			// �X�C���O�^�[�Q�b�g�̍��W��߂��B�L���͈͓��ɖ������nullptr��߂��B
			return retPosition;
		}

	}
}