#include "stdafx.h"
#include "AICharacterBase.h"
#include "ModelRender.h"
#include "NaviMesh.h"
#include "PathFinding.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		// AI�L�����N�^�[�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsAICharacterConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CAICharacterBase::Start()
		{
			// ���N���X�̊J�n����
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
			std::random_device rnd;
			// �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
			m_mt = std::make_unique<std::mt19937>(rnd());
			// [0, 99] �͈͂̈�l����
			m_rand = std::make_unique<std::uniform_int_distribution<>>(0, 99);
			// ���S�^�]���Ɏg�p����͈͕t���̈�l����
			m_randForSafeDrive = std::make_unique<std::uniform_int_distribution<>>(0, 2);

			// �h���N���X�̊J�n����
			return StartSub();
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CAICharacterBase::OnDestroy()
		{
			// �h���N���X�̏�������鎞�ɌĂ΂�鏈��
			OnDestroySub();

			// ���N���X�̏�������鎞�ɌĂ΂�鏈��
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CAICharacterBase::Update()
		{
			// ���N���X�̍X�V����

			// �h���N���X�̍X�V����
			UpdateSub();

			return;
		}

		/**
		 * @brief
		 * @param filePath
		 * @param pos
		 * @param rot
		 * @param scale
		*/
		void CAICharacterBase::InitModel(
			const char* filePath,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		)
		{
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);
			m_modelRender->Init(filePath);

			return;
		}

		/**
		 * @brief �p�X��������
		 * @param[in] targetPos �p�X�̖ڕW���W
		*/
		void CAICharacterBase::PathFinding(const Vector3& targetPos)
		{
			// �p�X����
			m_pathFindingRef->Execute(
				m_path,							// �\�z���ꂽ�p�X�̊i�[��
				*m_naviMeshRef,					// �i�r���b�V��
				m_modelRender->GetPosition() ,	// �J�n���W
				targetPos,						// �ړ��ڕW���W
				//PhysicsWorld::GetInstance(),	// �����G���W��
				nullptr,
				5.0f,							// AI�G�[�W�F���g�̔��a
				200.0f							// AI�G�[�W�F���g�̍����B
			);

			m_isEndPathMove = false;

			return;
		}

		/**
		 * @brief �^�[�Q�b�g�̒����烉���_���Ńp�X��������
		*/
		void CAICharacterBase::RandomTargetPathFinding()
		{
			const int randTargetNum = (*m_rand)(*m_mt);
			const Vector3 randTargetPos = (*m_naviMeshTargetPointsRef)[randTargetNum];

			PathFinding(randTargetPos);

			return;
		}

		/**
		 * @brief ���S�^�]��S�|�����p�X��������
		*/
		void CAICharacterBase::TargetPathFindingToSafeDrive()
		{
			// �^�[�Q�b�g�|�C���g�̌��
			std::unordered_map<EnCandidateTargetPointType,Vector3> candidateTargetPoints;
			// ���g����E�ܗp�̃^�[�Q�b�g�|�C���g�ւ̋���
			float posToTurnRightTPLen[enCandidateTargetPointTypeNum] = {};
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				switch (candidateTPType)
				{
				case enTurnRight:
					posToTurnRightTPLen[candidateTPType] = FLT_MIN;
					break;
				case enTurnLeft:
				case enGoStraight:
					posToTurnRightTPLen[candidateTPType] = FLT_MAX;
					break;
				}
			}

			// �^�[�Q�b�g�|�C���g�̒����玟�̈ړ��Ώۂ̃^�[�Q�b�g�|�C���g��T���o���B
			for (const auto& targetPos : *m_naviMeshTargetPointsRef)
			{
				// ���O���邩�H
				bool isExclude = false;

				// ���g�̍��W����^�[�Q�b�g���W�ւ̃x�N�g��
				const Vector3 posToTargetPosVec = targetPos - m_modelRender->GetPosition();
				// ���g�̍��W����^�[�Q�b�g���W�ւ̋���
				const float posToTargetPosLen = posToTargetPosVec.Length();
				// �ړ������ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
				float moveToPTTAngleRad = 0.0f;

				//////// 1.���ȏ�̋���������^�[�Q�b�g���W�����O���� ////////

				// �����ɂ��^�[�Q�b�g�|�C���g�̏��O
				isExclude = ExcludeForDistance(posToTargetPosLen);

				// ���O
				if (isExclude)
				{
					continue;
				}

				//////// 2. ���ȏ�̊p�x����̃^�[�Q�b�g�͏��O ////////

				// ���g�̍��W����^�[�Q�b�g���W�ւ̕����x�N�g��
				Vector3 posToTargetPosDir = posToTargetPosVec;
				posToTargetPosDir.Normalize();

				// �p�x�ɂ��^�[�Q�b�g�|�C���g�̏��O
				isExclude = ExcludeForAngle(&moveToPTTAngleRad, posToTargetPosDir);

				// ���O
				if (isExclude)
				{
					continue;
				}

				//////// 3.���ɓ����^�[�Q�b�g������Ό��ɓ���� ////////

				BeCandidateTargetPoint(
					&candidateTargetPoints,
					targetPos,
					posToTurnRightTPLen,
					posToTargetPosLen,
					posToTargetPosDir
				);
			}

			// ��₪������Ȃ������ꍇ�͉������Ȃ�
			if (candidateTargetPoints.empty())
			{
				return;
			}

			// �^�[�Q�b�g�|�C���g��T�������H
			bool IsFindedTargetPoint = false;
			EnCandidateTargetPointType randCandidateTPType = enTurnRight;

			while (IsFindedTargetPoint == false)
			{
				// �����_���ȃ^�[�Q�b�g�|�C���g�̔ԍ�
				int randTargetNum = (*m_randForSafeDrive)(*m_mt);
				randCandidateTPType = static_cast<EnCandidateTargetPointType>(randTargetNum);

				// ��������^�[�Q�b�g�|�C���g��T����
				if (candidateTargetPoints.count(randCandidateTPType))
				{
					IsFindedTargetPoint = true;
				}
			}

			PathFinding(candidateTargetPoints[randCandidateTPType]);

			return;
		}

		/**
		 * @brief �p�X����ړ�����
		 * @param[in] moveSpeed �ړ����x
		 * @param[in] physicsWorld �������[���h�B
		*/
		void CAICharacterBase::MoveOnPath(
			const float moveSpeed,
			const PhysicsWorld* physicsWorld
		)
		{
			if (m_isEndPathMove == true)
			{
				return;
			}

			const Vector3 posBeforeMove = m_modelRender->GetPosition();
			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				moveSpeed * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isEndPathMove,
				physicsWorld
			);
			Quaternion frontQRot = Quaternion::Identity;


			Rotating(&frontQRot, posBeforeMove, posAfterMove);

			m_modelRender->SetPosition(posAfterMove);
			m_modelRender->SetRotation(frontQRot);

			return;
		}

		/**
		 * @brief ����������
		 * @param[in] naviMeshRef �i�r���b�V��
		 * @param[in,,out] pathFindingRef �p�X��������
		*/
		void CAICharacterBase::Init(nsAI::SAICharacterInitData* AICharaInitData)
		{
			m_naviMeshRef = AICharaInitData->naviMeshRef;
			m_pathFindingRef = AICharaInitData->pathFindingRef;
			m_naviMeshTargetPointsRef = AICharaInitData->naviMeshTargetPointsRef;
		}

		/**
		 * @brief �ړ������ɉ�]����
		 * @param[in] frontQRotOut �O�����̉�]�̊i�[��
		 * @param[in] posBefoerMove �ړ��O�̍��W
		 * @param[in] posAfterMove �ړ���̍��W
		*/
		void CAICharacterBase::Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove)
		{
			m_moveDir = posAfterMove - posBeforeMove;
			m_moveDir.y = 0.0f;
			const float moveLenSq = m_moveDir.LengthSq();
			m_moveDir.Normalize();

			if (moveLenSq <= 10.0f)
			{
				return;
			}

			frontQRotOut->SetRotation(Vector3::Front, m_moveDir);

			return;
		}

		/**
		 * @brief �����ɂ��^�[�Q�b�g�|�C���g�̏��O
		 * @param distance ���g�ƃ^�[�Q�b�g�|�C���g�̋���
		 * @return ���O�������Htrue�߂��Ă����珜�O����
		*/
		bool CAICharacterBase::ExcludeForDistance(const float distance)
		{
			// ���������ȏ�̃^�[�Q�b�g�����O
			if (distance > kBetweenTargetPoint)
			{
				return true;
			}

			return false;
		}

		/**
		 * @brief �p�x�ɂ��^�[�Q�b�g�|�C���g�̏��O
		 * @param[out] moveToPTTAngleRad  �ړ������ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
		 * @param[in] posToTargetPosDir ���g����^�[�Q�b�g�ւ̃x�N�g��
		 * @return ���O�������Htrue���߂��Ă����珜�O����
		*/
		bool CAICharacterBase::ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir)
		{
			// �ړ������ƃ^�[�Q�b�g���W�ւ̕������ׂ�
			const float dotMoveAndPTTDir = Dot(m_moveDir, posToTargetPosDir);
			// �ړ������ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
			*moveToPTTAngleRadOut = acosf(dotMoveAndPTTDir);

			// ���̊p�x����̃^�[�Q�b�g�͑ΏۊO
			if (*moveToPTTAngleRadOut > kAngleThreshold)
			{
				return true;
			}

			return false;
		}

		/**
		 * @brief ���ɓ����^�[�Q�b�g�|�C���g�𒲂ׂ鏈��
		 * @param[in,out] candidateTPsOut �^�[�Q�b�g�|�C���g�̌��
		 * @param[in] targetPos �^�[�Q�b�g�|�C���g
		 * @param[in,out] posToCandidateTPLenOut ���g������̃^�[�Q�b�g�|�C���g�ւ̋���
		 * @param[in] posToTargetPosLen ���g����^�[�Q�b�g�|�C���g�ւ̋���
		 * @param[in] posToTargetPosDir ���g����^�[�Q�b�g�|�C���g�ւ̕���
		*/
		void CAICharacterBase::BeCandidateTargetPoint(
			std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
			const Vector3& targetPos,
			float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
			const float posToTargetPosLen,
			const Vector3& posToTargetPosDir
		)
		{
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				// �ڕW�ƂȂ�ړ���ւ̕���
				Vector3 moveTargetDir = Vector3::Front;
				switch (candidateTPType)
				{
				case enTurnRight:
					moveTargetDir = Cross(m_moveDir, Vector3::Up);
					break;
				case enTurnLeft:
					moveTargetDir = Cross(Vector3::Up, m_moveDir);
					break;
				case enGoStraight:
					moveTargetDir = m_moveDir;
					break;
				}

				moveTargetDir.Normalize();

				// �ڕW�ƂȂ�ړ���ւ̕����ƃ^�[�Q�b�g���W�ւ̕������ׂ�
				const float dotMoveTargetAndPTTDir = Dot(moveTargetDir, posToTargetPosDir);
				// �ڕW�ƂȂ�ړ���ւ̕����ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
				const float moveTargetToPTTAngleRad = acosf(dotMoveTargetAndPTTDir);

				// �ڕW�ƂȂ�ړ���ւ̕���������ȏ�p�x������^�[�Q�b�g�|�C���g�͑ΏۊO
				if (moveTargetToPTTAngleRad > kAngleThreshold)
				{
					continue;
				}

				// �n�߂Ă̌��̏ꍇ
				if (candidateTPsOut->count(enTurnRight) == 0)
				{
					// ���ɂ���
					candidateTPsOut->insert({ enTurnRight, targetPos });
					// ������ێ����Ă���
					*posToCandidateTPLenOut = posToTargetPosLen;
					
					// ���܂������珈�����I������
					return;
				}

				// ���Ɍ�₪����ꍇ

				// ���̃^�[�Q�b�g�|�C���g���X�V���邩�H
				bool doUpdateCandidateTP = false;
				switch (candidateTPType)
				{
				case enTurnRight:
					if (posToTargetPosLen > *posToCandidateTPLenOut)
					{
						// �E�܂͉����������ɂ���
						doUpdateCandidateTP = true;
					}
					break;
				case enTurnLeft:
				case enGoStraight:
					if (posToTargetPosLen <= *posToCandidateTPLenOut)
					{
						// ���܂ƒ��i�͉����������ɂ���
						doUpdateCandidateTP = true;
					}
					break;
				}

				if (doUpdateCandidateTP)
				{
					// �E�܂�����A�����������ɂ���
					(*candidateTPsOut)[enTurnRight] = targetPos;
					// �������X�V����
					*posToCandidateTPLenOut = posToTargetPosLen;

					// ���܂������珈�����I������
					return;
				}

			}

			return;
		}

	}
}