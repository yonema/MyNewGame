#include "stdafx.h"
#include "AICar.h"
#include "AICharacterConstData.h"
#include "ModelRender.h"
#include "Level3D.h"
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
		 * @brief �R���X�g���N�^
		*/
		CAICar::CAICar()
		{
			// ���������u�ԂɌĂ�łق�������

			// ���f�������_���[�̐���
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
		}

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CAICar::Start()
		{
			// ���f���̏�����
			m_modelRender->Init(kCarModelFilePath);

			// �����̏�����
			InitRand();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CAICar::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CAICar::Update()
		{
			if (m_isMoveEnd)
			{
				// �ړ��I��

				// ���ɐi�ރE�F�C�|�C���g��T��
				FindNextWayPoint();
			}
			else
			{
				// �ړ���

				// �E�F�C�|�C���g����ړ�����
				MoveOnWayPoint();
			}

			return;
		}

		/**
		 * @brief ����������
		 * @param[in] AICharaInitData AI�L�����N�^�[�̏������f�[�^
		 * @param[in] pos ���W
		 * @param[in] rot ��]
		 * @param[in] scale �g��
		*/
		void CAICar::Init(
			const nsAI::SAICharacterInitData& AICharaInitData,
			const Vector3& pos,
			const Quaternion rot,
			const Vector3& scale
		)
		{
			// �E�F�C�|�C���g�̎Q�Ƃ𓾂�
			m_wayPointsRef = AICharaInitData.naviMeshTargetPointsRef;

			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);

			return;
		}

		/**
		 * @brief �����̏�����
		*/
		void CAICar::InitRand()
		{
			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
			std::random_device rnd;
			// �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
			m_mt = std::make_unique<std::mt19937>(rnd());
			// �͈͂̈�l����
			m_rand = std::make_unique<std::uniform_int_distribution<>>(0, enCandidateTargetPointTypeNum - 1);

			return;
		}

		/**
		 * @brief ���̃E�F�C�|�C���g��T��
		*/
		void CAICar::FindNextWayPoint()
		{
			// �^�[�Q�b�g�|�C���g�̌��
			std::unordered_map<EnCandidateTargetPointType, Vector3> candidateTargetPoints;
			// ���g����^�[�Q�b�g�|�C���g�̌��ւ̋���
			float posToCandidateTPLen[enCandidateTargetPointTypeNum] = {};
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				switch (candidateTPType)
				{
				case enTurnRight:
					posToCandidateTPLen[candidateTPType] = FLT_MIN;
					break;
				case enTurnLeft:
				case enGoStraight:
					posToCandidateTPLen[candidateTPType] = FLT_MAX;
					break;
				}
			}

			// �^�[�Q�b�g�|�C���g�̒����玟�̈ړ��Ώۂ̃^�[�Q�b�g�|�C���g��T���o���B
			for (const auto& targetPos : *m_wayPointsRef)
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
					posToCandidateTPLen,
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
				int randTargetNum = (*m_rand)(*m_mt);
				randCandidateTPType = static_cast<EnCandidateTargetPointType>(randTargetNum);

				// ��������^�[�Q�b�g�|�C���g��T����
				if (candidateTargetPoints.count(randCandidateTPType))
				{
					IsFindedTargetPoint = true;
				}
			}

			// �ړ��I���t���O�����Z�b�g����
			m_isMoveEnd = false;

			// �p�X���\�z
			PathBuilding(randCandidateTPType, candidateTargetPoints[randCandidateTPType]);

			return;
		}

		/**
		 * @brief �E�F�C�|�C���g����ړ�����
		*/
		void CAICar::MoveOnWayPoint()
		{
			const Vector3 posBeforeMove = m_modelRender->GetPosition();

			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				2000.0f * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isMoveEnd,
				PhysicsWorld::GetInstance()
			);

			const Vector3 moveVec = posAfterMove - posBeforeMove;
			m_moveDir = moveVec;
			m_moveDir.Normalize();

			m_modelRender->SetPosition(posAfterMove);
			Quaternion qRot;
			qRot.SetRotation(Vector3::Front, m_moveDir);
			m_modelRender->SetRotation(qRot);

		}

		/**
		 * @brief �����ɂ��^�[�Q�b�g�|�C���g�̏��O
		 * @param distance ���g�ƃ^�[�Q�b�g�|�C���g�̋���
		 * @return ���O�������Htrue�߂��Ă����珜�O����
		*/
		bool CAICar::ExcludeForDistance(const float distance)
		{
			// ���������ȏ�̃^�[�Q�b�g�����O
			if (distance > kBetweenTargetPoint)
			{
				return true;
			}

			// ���������ȉ�
			if (distance < kMinBetweenTargetPoint)
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
		bool CAICar::ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir)
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
		void CAICar::BeCandidateTargetPoint(
			std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
			const Vector3& targetPos,
			float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
			const float posToTargetPosLen,
			const Vector3& posToTargetPosDir
		)
		{
			for (int candidateTPTypeNum = 0; candidateTPTypeNum < enCandidateTargetPointTypeNum; candidateTPTypeNum++)
			{
				// �ڕW�ƂȂ�ړ���ւ̕���
				Vector3 moveTargetDir = Vector3::Front;
				float angleThreshold = kAngleThreshold;
				EnCandidateTargetPointType candidateTPType = static_cast<EnCandidateTargetPointType>(candidateTPTypeNum);

				switch (candidateTPType)
				{
				case enTurnRight:
					// moveTargetDir = Cross(m_moveDir, Vector3::Up);
					moveTargetDir = Cross(Vector3::Up, m_moveDir);
					break;
				case enTurnLeft:
					// moveTargetDir = Cross(Vector3::Up, m_moveDir);
					moveTargetDir = Cross(m_moveDir, Vector3::Up);
					break;
				case enGoStraight:
					moveTargetDir = m_moveDir;
					angleThreshold = kStraightAngleThreshold;
					break;
				}

				moveTargetDir.Normalize();

				// �ڕW�ƂȂ�ړ���ւ̕����ƃ^�[�Q�b�g���W�ւ̕������ׂ�
				const float dotMoveTargetAndPTTDir = Dot(moveTargetDir, posToTargetPosDir);
				// �ڕW�ƂȂ�ړ���ւ̕����ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
				const float moveTargetToPTTAngleRad = acosf(dotMoveTargetAndPTTDir);

				// �ڕW�ƂȂ�ړ���ւ̕���������ȏ�p�x������^�[�Q�b�g�|�C���g�͑ΏۊO
				if (moveTargetToPTTAngleRad > angleThreshold)
				{
					continue;
				}

				// �n�߂Ă̌��̏ꍇ
				if (candidateTPsOut->count(candidateTPType) == 0)
				{
					// ���ɂ���
					candidateTPsOut->insert({ candidateTPType, targetPos });
					// ������ێ����Ă���
					posToCandidateTPLenOut[candidateTPType] = posToTargetPosLen;

					// ���܂������珈�����I������
					return;
				}

				// ���Ɍ�₪����ꍇ

				// ���̃^�[�Q�b�g�|�C���g���X�V���邩�H
				bool doUpdateCandidateTP = false;
				switch (candidateTPType)
				{
				case enTurnRight:
					if (posToTargetPosLen > posToCandidateTPLenOut[candidateTPType])
					{
						// �E�܂͉����������ɂ���
						doUpdateCandidateTP = true;
					}
					break;
				case enTurnLeft:
				case enGoStraight:
					if (posToTargetPosLen <= posToCandidateTPLenOut[candidateTPType])
					{
						// ���܂ƒ��i�͋߂��������ɂ���
						doUpdateCandidateTP = true;
					}
					break;
				}

				if (doUpdateCandidateTP)
				{
					(*candidateTPsOut)[candidateTPType] = targetPos;
					// �������X�V����
					posToCandidateTPLenOut[candidateTPType] = posToTargetPosLen;

					// ���܂������珈�����I������
					return;
				}

			}

			return;
		}

		/**
		 * @brief �p�X���\�z����
		 * @param candidateTPType �^�[�Q�b�g�|�C���g�ւ̌��̎��
		 * @param targetPoint �^�[�Q�b�g�|�C���g
		*/
		void CAICar::PathBuilding(
			const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
			const Vector3& targetPoint
		)
		{
			// �p�X���N���A
			m_path.Clear();

			// �X�^�[�g�n�_��ǉ�
			m_path.AddPoint(m_modelRender->GetPosition());

			if (candidateTPType == enTurnLeft || candidateTPType == enTurnRight)
			{
				// �E�܂����܂Ȃ璆�p�n�_��ǉ�����

				// �^�[�Q�b�g�|�C���g�ւ̃x�N�g��
				const Vector3 toTargetPointVec = targetPoint - m_modelRender->GetPosition();
				// �^�[�Q�b�g�|�C���g�ւ̃x�N�g�����ˉe���ꂽ�ړ������x�N�g��
				const float toTPVecProjMoveDir = Dot(m_moveDir, toTargetPointVec);
				// �^�[�Q�b�g�|�C���g�̒��p�n�_
				const Vector3 relayTargetPos = 
					m_modelRender->GetPosition() +m_moveDir * toTPVecProjMoveDir;
				// ���p�n�_��ǉ�
				m_path.AddPoint(relayTargetPos);
			}

			// �S�[���n�_��ǉ�
			m_path.AddPoint(targetPoint);

			// �p�X���\�z
			m_path.Build();

			return;
		}

	}
}