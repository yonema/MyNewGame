#include "stdafx.h"
#include "AICar.h"
#include "AICharacterConstData.h"
#include "ModelRender.h"
#include "Level3D.h"
#include "GameTime.h"
#include "GameMainState.h"
#include "BezierCurve.h"


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
			// �Q�[���X�e�[�g�ɎԂ�o�^
			nsGameState::CGameMainState::GetInstance()->AddAICar(this);
		}

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CAICar::Start()
		{
			// ���f���̏�����
			m_modelRender->Init(kCarModelFilePath);

			// �Ԃ̈ړ����x��ݒ�
			m_moveSpeed = kCarSpeed;

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

			// �ړ���

			// �E�F�C�|�C���g����ړ�����
			MoveOnWayPoint();

			// ��]���X�V
			UpdateRotating();


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
			// �ړ��O�̍��W
			const Vector3 posBeforeMove = m_modelRender->GetPosition();

			// �p�X�ړ���̍��W
			// �p�X�ړ�
			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				m_moveSpeed * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isMoveEnd,
				PhysicsWorld::GetInstance()
			);

			// �ړ��������v�Z
			m_moveDir = posAfterMove - posBeforeMove;;
			m_moveDir.Normalize();

			// ���f���̍��W���X�V
			m_modelRender->SetPosition(posAfterMove);

		}


		/**
		 * @brief ��]���X�V
		*/
		void CAICar::UpdateRotating()
		{
			// ��]����������x�N�g��
			Vector3 rotateVec = Vector3::Front;
			// ���݂̃Z�N�V�����̃^�O
			nsAI::CPath::EnSectionTag sectionTag = m_path.GetCurrentSectionTag();
			m_moveSpeed = kCarSpeed;

			if (sectionTag == nsAI::CPath::enLeftCurveStart ||
				sectionTag == nsAI::CPath::enRightCurveStart/* ||
				sectionTag == nsAI::CPath::enLeftCurveEnd ||
				sectionTag == nsAI::CPath::enRightCurveEnd*/)
			{
				// �Z�N�V�������A�J�[�u�̊J�n�n�_�A�܂��́A�J�[�u�̏I���n�_

				// ���݂̈ړ��̕��������Ɍv�Z����
				rotateVec = m_moveDir;
				// �h���t�g�̉�]
				Quaternion driftRot = Quaternion::Identity;
				// �Z�N�V�����̊J�n���W���猻�݂̍��W�ւ̃x�N�g��
				const Vector3 fromSectionStartPos =
					m_modelRender->GetPosition() - m_path.GetCurrentSection().startPos;
				// ���݁A�Z�N�V�����̉����̈ʒu�ɂ��邩���A�h���t�g�̉�]���Ƃ���B
				float driftRate = fromSectionStartPos.Length() / m_path.GetCurrentSection().length;
				// �ŏ��h���t�g�̊p�x
				float minDriftAngle = 0.0f;
				// �ő�h���t�g�̊p�x
				float maxDriftAngle = kCarDriftMaxAngle;
				// �J�[�u�I���Z�N�V�����Ȃ�A�ŏ��ƍő�𔽓]����
				if (sectionTag == nsAI::CPath::enLeftCurveEnd || sectionTag == nsAI::CPath::enRightCurveEnd)
				{
					minDriftAngle = kCarDriftMaxAngle;
					maxDriftAngle = 0.0f;
				}
				// �h���t�g���ɉ����āA�h���t�g�̊p�x����`�⊮����B
				float driftAngle = Math::Lerp<float>(min(driftRate, 1.0f), minDriftAngle, maxDriftAngle);
				
				// ���܂Ȃ�A�p�x�𔽓]������B
				if (sectionTag == nsAI::CPath::enLeftCurveStart || sectionTag == nsAI::CPath::enLeftCurveEnd)
				{
					driftAngle = -driftAngle;
				}
				driftRot.SetRotationDegY(driftAngle);
				driftRot.Apply(rotateVec);
			}
			else if (sectionTag == nsAI::CPath::enLeftCurve ||
				sectionTag == nsAI::CPath::enRightCurve)
			{
				// �Z�N�V�������A�J�[�u���̂Ƃ�

				// �J�[�u�̒��S���W����A���݂̍��W�ւ̕����x�N�g���B
				Vector3 fromCurveCenterPosNorm = m_modelRender->GetPosition() - m_curveCenterPosition;
				fromCurveCenterPosNorm.Normalize();
				// �ړ��������ƁA��]�ɃJ�N�c�L���ł��Ă��܂����߁A���S����̃x�N�g�������Ɍv�Z����B
				rotateVec = fromCurveCenterPosNorm;

				// �J�[�u�̒��S����̊p�x��
				float angleRate = Dot(fromCurveCenterPosNorm, m_fromCurveCenterToCurveStartVec);
				angleRate = acosf(angleRate);
				// 0�x�`90�x���A0.0f�`1.0f�ɂȂ�B
				angleRate = angleRate / (3.14f * 0.5f);
				// �h���t�g�ɉ������]
				float addAngle = 0.0f;

				// �h���t�g�̉�]�����Ƃɖ߂�
				if (angleRate >= kCarDriftTurnBackRate)
				{
					// ��ԗ��B0.0f�`(1.0f-kCarDriftTurnBackRate)�̒l���A0.0f�`1.0f�̒l�ɒ����B
					float t = (angleRate - kCarDriftTurnBackRate) / kCarDriftTurnBackRate;
					addAngle = Math::Lerp<float>(t, 0.0f, kCarDriftMaxAngle);
				}

				// �h���t�g���̃X�s�[�h�_�E��
				if (angleRate <= kCarDriftSpeedDownRate)
				{
					// �w���֐��I�ɕω��ɂ���B
					float t = 1.0f - (angleRate / kCarDriftSpeedDownRate);
					t = 1.0f - pow(t, 2.0f);
					m_moveSpeed = Math::Lerp<float>(min(t,1.0f),  kCarSpeed, kCarSpeed * kCarDriftSpeedDownScale);
				}
				else
				{
					// �w���œI�ω��ɂ���B
					float t = ((angleRate - kCarDriftSpeedDownRate) / kCarDriftSpeedDownRate);
					t = pow(t, 2.0f);
					m_moveSpeed = Math::Lerp<float>(min(t, 1.0f), kCarSpeed * kCarDriftSpeedDownScale, kCarSpeed);
				}

				// �J�[�u�̉�]
				Quaternion curveRot = Quaternion::Identity;

				// ���S����̃x�N�g���𒼊p�ɋȂ��āA����Ƀh���t�g���̊p�x������������]���v�Z����B
				if (m_path.GetCurrentSectionTag() == nsAI::CPath::enLeftCurve)
				{
					// ���܎�
					curveRot.SetRotationDegY(-90.0f - kCarDriftMaxAngle + addAngle);
				}
				else if (m_path.GetCurrentSectionTag() == nsAI::CPath::enRightCurve)
				{
					// �E�܎�
					curveRot.SetRotationDegY(90.0f + kCarDriftMaxAngle - addAngle);

				}
				curveRot.Apply(rotateVec);
			}
			else
			{
				// �ʏ�͈ړ������ɉ�]������
				rotateVec = m_moveDir;
			}

			// ���̉�]
			Quaternion nextQRot;
			// ���̉�]���v�Z
			nextQRot.SetRotation(Vector3::Front, rotateVec);
			// �J�N�c�L��}���邽�߂ɁA���ʐ��`�⊮���s���B
			nextQRot.Slerp(kCarModelRotRate, m_modelRender->GetRotation(), nextQRot);
			// ���f���ɉ�]��ݒ�
			m_modelRender->SetRotation(nextQRot);

			return;
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

				// �J�[�u�̍��W���쐬����
				MakeCurve(candidateTPType, targetPoint);
			}

			// �S�[���n�_��ǉ�
			m_path.AddPoint(targetPoint);

			// �p�X���\�z
			m_path.Build();

			return;
		}

		/**
		 * @brief �J�[�u�̍��W���쐬����
		 * @param candidateTPType �^�[�Q�b�g�|�C���g�ւ̌��̎��
		 * @param targetPoint �^�[�Q�b�g�|�C���g
		*/
		void CAICar::MakeCurve(
			const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
			const Vector3& targetPoint
		)
		{
			// �^�[�Q�b�g�|�C���g�ւ̃x�N�g��
			const Vector3 toTargetPointVec = targetPoint - m_modelRender->GetPosition();
			// �^�[�Q�b�g�|�C���g�ւ̃x�N�g�����ˉe���ꂽ�ړ������x�N�g��
			const float toTPVecProjMoveDir = Dot(m_moveDir, toTargetPointVec);

			enum EnControlPointType
			{
				enStartCP,
				enCenterCP,
				enMidleCP,
				enEndCP,
				enCPNum
			};

			// �x�W�F�Ȑ��̐���_�̍��W
			Vector3 controlPoints[enCPNum];
			// �^�񒆂̐���_
			controlPoints[enCenterCP] = m_modelRender->GetPosition() + m_moveDir * toTPVecProjMoveDir;

			// �^�񒆂���ŏ��̐���_�ւ̃x�N�g��
			Vector3 cCpToScpVec = m_modelRender->GetPosition() - controlPoints[enCenterCP];
			cCpToScpVec.Normalize();
			// �^�񒆂���Ō�̐���_�ւ̃x�N�g��
			Vector3 cCpToEcpVec = targetPoint - controlPoints[enCenterCP];
			cCpToEcpVec.Normalize();

			// ����_�Ԃ̒���
			float cpLen = 875.0f;
			// ���܂Ȃ琧��_�Ԃ̒������Z��

			if (candidateTPType == enTurnRight)
			{
				// �E�܂Ȃ琧��_�Ԃ̒���������
				cpLen = 1050.0f;
			}
			
			// ����_�ւ̃x�N�g����L�΂�
			cCpToScpVec.Scale(cpLen);
			cCpToEcpVec.Scale(cpLen * 1.5f);

			// ��O�Ɖ��̐���_�̍��W���v�Z����
			controlPoints[enStartCP] = controlPoints[enCenterCP] + cCpToScpVec;
			controlPoints[enEndCP] = controlPoints[enCenterCP] + cCpToEcpVec;
			controlPoints[enMidleCP] =
				controlPoints[enEndCP] + (cCpToScpVec * 0.1f) + (cCpToEcpVec * -0.5f);

			// �J�[�u���̒��S���W���v�Z
			m_curveCenterPosition = controlPoints[enStartCP] + cCpToEcpVec;
			// �J�[�u���̒��S���W����J�[�u�J�n�n�_�ւ̍��W���v�Z
			m_fromCurveCenterToCurveStartVec = cCpToEcpVec;
			m_fromCurveCenterToCurveStartVec.Scale(-1.0f);
			m_fromCurveCenterToCurveStartVec.Normalize();

			// �J�[�u�̊J�n�n�_�ƏI���n�_���v�Z
			Vector3 curveStartPos = controlPoints[enCenterCP] + (cCpToScpVec * 3.0f);
			Vector3 curveEndPos = controlPoints[enCenterCP] + (cCpToEcpVec * 1.5f);

			// �^�񒆂̐���_���ړ�������
			controlPoints[enCenterCP] += (cCpToScpVec * -1.0f) + (cCpToEcpVec * 0.1f);
			//controlPoints[enStartCP] += (cCpToEcpVec * -0.2f);

			// �x�W�F�Ȑ��N���X
			nsCurve::CBezierCurve bezierCurve;
			// �x�W�F�Ȑ��ɐ���_��ǉ�����
			for (int i = 0; i < enCPNum; i++)
			{
				bezierCurve.AddControlPoint(controlPoints[i]);
			}
			
			// �p�X�̃Z�N�V�����̃^�O
			nsAI::CPath::EnSectionTag startST = nsAI::CPath::enLeftCurveStart;
			nsAI::CPath::EnSectionTag sectionTag = nsAI::CPath::enLeftCurve;
			nsAI::CPath::EnSectionTag endST = nsAI::CPath::enLeftCurveEnd;

			if (candidateTPType == enTurnRight)
			{
				// �E�܂Ȃ�E�ܗp�̃^�O
				startST = nsAI::CPath::enRightCurveStart;
				sectionTag = nsAI::CPath::enRightCurve;
				endST = nsAI::CPath::enRightCurveEnd;
			}

			// ���p�n�_���p�X�ɒǉ�
			
			// �J�[�u�J�n�n�_��ǉ�
			m_path.AddPoint(curveStartPos, startST);
			// �J�[�u���̒n�_��ǉ�
			for (int i = 0; i < kCurveRelayPointNum; i++)
			{
				if (i == (kCurveRelayPointNum - 1))
				{
					// �Ō�̃J�[�u�n�_�ɂ́A�J�[�u�I���̃^�O�𖄂ߍ���
					sectionTag = endST;
				}
				// ���p�n�_
				Vector3 relayPos = Vector3::Zero;
				// �x�W�F�Ȑ��̃p�����[�^
				const float t = static_cast<float>(i) / static_cast<float>(kCurveRelayPointNum - 1);
				// �x�W�F�Ȑ����p�����[�^���w�肵�Čv�Z
				bezierCurve.CalcBezierCurve(t, &relayPos);
				// ���p�n�_���p�X�ɒǉ�
				m_path.AddPoint(relayPos, sectionTag);
			}
			// �J�[�u�I���̃Z�N�V�����̏I�_��ǉ�
			m_path.AddPoint(curveEndPos, nsAI::CPath::enStraight);

		}

	}
}