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
		 * @brief �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�ɃN�G�������s
		 * @param[in] fromPos �w����W
		 * @param[in] scopeRadius �L���͈�
		 * @param[in] func ���s����֐�
		*/
		void QuerySwingTargetWithinScope(
			const Vector3& fromPos,
			const float scopeRadius,
			const std::function<void(nsSwingTarget::CSwingTarget* swingTarget)>func
		) 
		{
			// Y���W�i�����j�𖳎�����XZ���ʂł̍��W
			Vector3 fromPosXZ = fromPos;
			fromPosXZ.y = 0.0f;

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

					//////// 2.�L���͈͓��Ȃ̂ŁA�֐������s ////////

					// �֐������s
					func(swingTarget);

					return;
				}
			);

			return;
		}


		/**
		 * @brief �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W��T���Ă���
		 * @param[in] data ��ԋ߂��X�C���O�^�[�Q�b�g�̃|�C���g�̍��W��T�������̃f�[�^
		 * @return �X�C���O�^�[�Q�b�g�̍��W�B�L���͈͓��ɖ������nullptr��߂��B
		*/
		const Vector3* const FindNearestSwingTargetPoint(
			const SFindNearestSwingTargetPointData& data
		)
		{
			// �ŒZ������ێ����Ă����ϐ�
			float tmpDist = FLT_MAX;

			// �߂�l�p�̍��W�̃|�C���^
			const Vector3* retPosition = nullptr;

			// �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�ɃN�G�������s
			QuerySwingTargetWithinScope(
				data.findSwingTargetOrigin,
				data.findSwingTargetScopeRadius,
				[&](nsSwingTarget::CSwingTarget* swingTarget)
				{
					// �X�C���O�^�[�Q�b�g�̍��W�R���e�i���擾
					const std::vector<Vector3>& stPoints = swingTarget->GetSwingTargetPositions();

					// �S�Ă̍��W�𒲂ׂ�
					for (auto& stp : stPoints)
					{
						// ���_����X�C���O�^�[�Q�b�g�|�C���g�̋����x�N�g��
						Vector3 findSTPOriginToSTPVec = stp - data.findSwingTargetPointOrigin;

						// ���_����X�C���O�^�[�Q�b�g�|�C���g�̋���
						const float findSTPOriginToSTPVecLen = findSTPOriginToSTPVec.Length();

						if (findSTPOriginToSTPVecLen > data.findSwingTargetPointScopeRadius)
						{
							// �X�C���O�^�[�Q�b�g�|�C���g���L���͈͊O�B���ցB
							continue;
						}

						if (stp.y < data.heightLowerLimit)
						{
							// �X�C���O�^�[�Q�b�g�|�C���g�̍�����������菬�����B���ցB
							continue;
						}

						// ���_����X�C���O�^�[�Q�b�g�|�C���g�̋��������x�N�g��
						Vector3 flOriginToSTPDir = stp - data.forwardLimitOrigin;
						flOriginToSTPDir.Normalize();	// ���K������

						if (Dot(flOriginToSTPDir, data.forwardLimitDir) < 0.0f)
						{
							// �X�C���O�^�[�Q�b�g�|�C���g���O�����̐��������ɂ���B���ցB
							continue;
						}

						// ����̋��������܂ł̍ŒZ�������Z�����H
						if (findSTPOriginToSTPVecLen < tmpDist)
						{
							// �Z���ꍇ�͂��̋����Ƃ��̍��W��ێ�
							tmpDist = findSTPOriginToSTPVecLen;
							retPosition = &stp;
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