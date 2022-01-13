#pragma once
#include "AICharacterBase.h"
#include "AICharacterConstData.h"
#include "AIField.h"
#include "Path.h"


namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}
	namespace nsSound { class CSoundCue; }


	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AI�̎ԃN���X
		*/
		class CAICar : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAICar();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CAICar() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroy() override final;

			/**
			 * @brief �X�V����
			*/
			void Update() override final;

		public:		// �����o�֐�

			/**
			 * @brief ����������
			 * @param[in] AICharaInitData AI�L�����N�^�[�̏������f�[�^
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g��
			*/
			void Init(
				const nsAI::SAICharacterInitData& AICharaInitData,
				const Vector3& pos, 
				const Quaternion rot, 
				const Vector3& scale
			);

			/**
			 * @brief ���W���擾
			 * @return ���W
			*/
			const Vector3& GetPosition() const
			{
				return m_modelRender->GetPosition();
			}

			/**
			 * @brief ��]���擾
			 * @return ��]
			*/
			const Quaternion& GetRotation() const
			{
				return m_modelRender->GetRotation();
			}

			/**
			 * @brief �߂܂�����Ԃɂ���
			*/
			void BeCaptured()
			{
				m_isCaptured = true;
			}

			/**
			 * @brief �߂܂��Ă��邩�H�𓾂�
			 * @return �߂܂��Ă��邩�H
			*/
			bool IsCaputred() const
			{
				return m_isCaptured;
			}

			/**
			 * @brief �E�p�̃G�t�F�N�g�̎Q�Ƃ�ݒ肷��
			 * @param[in.out] effectRef �E�p�̃G�t�F�N�g�̎Q��
			*/
			void SetNinjyutuEffectRef(Effect* effectRef)
			{
				m_ninjyutuEFRef = effectRef;
			}

		private:	// private�ȃf�[�^�����o

			/**
			 * @brief �����̏�����
			*/
			void InitRand();

			/**
			 * @brief �G�t�F�N�g�̏�����
			*/
			void InitEffect();

			/**
			 * @brief �T�E���h�̏�����
			*/
			void InitSound();

			/**
			 * @brief �߂܂��Ă��鎞�̍X�V
			 * @return ���̌���X�V�������s�����H
			*/
			bool UpdateOnIsCaptured();

			/**
			 * @brief ���̃G�t�F�N�g�̍X�V
			*/
			void UpdateSmokeEffect();

			/**
			 * @brief ���̃E�F�C�|�C���g��T��
			*/
			void FindNextWayPoint();

			/**
			 * @brief �E�F�C�|�C���g����ړ�����
			*/
			void MoveOnWayPoint();

			/**
			 * @brief ��]���X�V
			*/
			void UpdateRotating();

			/**
			 * @brief �T�E���h���X�V
			*/
			void UpdateSound();

			/**
			 * @brief �T�E���h�̉��ʂ��v�Z����
			 * @return �v�Z���ꂽ�T�E���h�̉���
			*/
			float CalcSoundVolume();

			/**
			 * @brief ���̃N���X�̃T�E���h�̒�~
			*/
			void StopSound();

			/**
			 * @brief �����ɂ��^�[�Q�b�g�|�C���g�̏��O
			 * @param[in] distance ���g�ƃ^�[�Q�b�g�|�C���g�̋���
			 * @return ���O�������Htrue�߂��Ă����珜�O����
			*/
			bool ExcludeForDistance(const float distance);

			/**
			 * @brief �p�x�ɂ��^�[�Q�b�g�|�C���g�̏��O
			 * @param[out] moveToPTTAngleRad  �ړ������ƃ^�[�Q�b�g���W�ւ̕����̃��W�A���p
			 * @param[in] posToTargetPosDir ���g����^�[�Q�b�g�ւ̃x�N�g��
			 * @return ���O�������Htrue���߂��Ă����珜�O����
			*/
			bool ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir);

			/**
			 * @brief ���ɓ����^�[�Q�b�g�|�C���g�𒲂ׂ鏈��
			 * @param[in,out] candidateTPsOut �^�[�Q�b�g�|�C���g�̌��
			 * @param[in] targetPos �^�[�Q�b�g�|�C���g
			 * @param[in,out] posToCandidateTPLenOut ���g������̃^�[�Q�b�g�|�C���g�ւ̋���
			 * @param[in] posToTargetPosLen ���g����^�[�Q�b�g�|�C���g�ւ̋���
			 * @param[in] posToTargetPosDir ���g����^�[�Q�b�g�|�C���g�ւ̕���
			*/
			void BeCandidateTargetPoint(
				std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
				const Vector3& targetPos,
				float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
				const float posToTargetPosLen,
				const Vector3& posToTargetPosDir
			);

			/**
			 * @brief �p�X���\�z����
			 * @param candidateTPType �^�[�Q�b�g�|�C���g�ւ̌��̎��
			 * @param targetPoint �^�[�Q�b�g�|�C���g
			*/
			void PathBuilding(
				const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
				const Vector3& targetPoint
			);

			/**
			 * @brief �J�[�u�̍��W���쐬����
			 * @param candidateTPType �^�[�Q�b�g�|�C���g�ւ̌��̎��
			 * @param targetPoint �^�[�Q�b�g�|�C���g
			*/
			void MakeCurve(
				const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
				const Vector3& targetPoint
			);


		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			Vector3 m_moveDir = Vector3::Front;							//!< �ړ�����
			const std::vector<Vector3>* m_wayPointsRef = nullptr;		//!< �E�F�C�|�C���g�̎Q��
			nsAI::CPath m_path;											//!< �p�X
			bool m_isMoveEnd = true;									//!< �ړ����I���������H
			Vector3 m_curveCenterPosition = Vector3::Zero;				//!< �J�[�u�̒��S���W
			Vector3 m_fromCurveCenterToCurveStartVec = Vector3::Zero;	//!< �J�[�u���S���W����J�[�u�J�n���W�ւ̃x�N�g��
			float m_moveSpeed = 0.0f;
			bool m_isCaptured = false;									//!< �߂܂������H
			Effect* m_explotionEF = nullptr;							//!< �����̃G�t�F�N�g
			Effect* m_smokeEFs[nsAICharacterConstData::kSmokeNum] = {};	//!< ���̃G�t�F�N�g
			Effect* m_smokeExplotionEF = nullptr;						//!< �������̉��̃G�t�F�N�g
			float m_isCaptureTimer = 0.0f;								//!< �߂܂��Ă��鎞�̃^�C�}�[
			Effect* m_ninjyutuEFRef = nullptr;							//!< �E�p�̃G�t�F�N�g�̎Q��
			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< �͈͕t���̈�l����
			std::unique_ptr<std::mt19937> m_mt;							//!< �����Z���k�c�C�X�^�[��32�r�b�g��

			// �T�E���h
			nsSound::CSoundCue* m_driveSC = nullptr;		//!< �^�]���̃T�E���h
			nsSound::CSoundCue* m_driftSC = nullptr;		//!< �h���t�g�̃T�E���h
			nsSound::CSoundCue* m_fireExplosionSC = nullptr;	//!< �Γق̔����̃T�E���h
			nsSound::CSoundCue* m_carExplosionSC = nullptr;		//!< �Ԃ̔����̃T�E���h
			bool m_driftSoundFlag = false;					//!< �h���t�g�����Đ��ł��邩�H
			float m_driveTimer = 0.0f;						//!< �^�]���Ɏg�p����^�C�}�[
		};

	}
}