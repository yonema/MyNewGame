#pragma once
#include "AICharacterConstData.h"
#include "Path.h"
#include "AIField.h"


namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}
	namespace nsAI {
		class CNaviMesh;
		class CPathFinding;
		struct SAICharacterInitData;
	}


	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AI�L�����N�^�[�̊��N���X
		*/
		class CAICharacterBase : public IGameObject
		{
		public:		// �\����

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAICharacterBase() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CAICharacterBase() = default;

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

		protected:	// ���z�֐�

			/**
			 * @brief �h���N���X�p��Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			virtual bool StartSub() { return true; };

			/**
			 * @brief �h���N���X�p�̏�������鎞�ɌĂ΂�鏈��
			*/
			virtual void OnDestroySub() {};

			/**
			 * @brief �h���N���X�p�̍X�V����
			*/
			virtual void UpdateSub() {};

		public:		// �����o�֐�

			/**
			 * @brief ����������
			 * @param[in] naviMeshRef �i�r���b�V��
			 * @param[in,,out] pathFindingRef �p�X��������
			*/
			void Init(nsAI::SAICharacterInitData* AICharaInitData);

			/**
			 * @brief �ړ������ɉ�]����
			 * @param[in] frontQRotOut �O�����̉�]�̊i�[��
			 * @param[in] posBefoerMove �ړ��O�̍��W
			 * @param[in] posAfterMove �ړ���̍��W
			*/
			void Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove);

		protected:	// protected�ȃ����o�֐�

			/**
			 * @brief 
			 * @param filePath 
			 * @param pos 
			 * @param rot 
			 * @param scale 
			*/
			void InitModel(
				const char* filePath,
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3& scale
			);

			/**
			 * @brief �p�X��������
			 * @param[in] targetPos �p�X�̖ڕW���W
			*/
			void PathFinding(const Vector3& targetPos);

			/**
			 * @brief �^�[�Q�b�g�̒����烉���_���Ńp�X��������
			*/
			void RandomTargetPathFinding();

			/**
			 * @brief ���S�^�]��S�|�����p�X��������
			*/
			void TargetPathFindingToSafeDrive();

			/**
			 * @brief �p�X����ړ�����
			 * @param[in] moveSpeed �ړ����x
			 * @param[in] physicsWorld �������[���h�B
			*/
			void MoveOnPath(
				const float moveSpeed,
				const PhysicsWorld* physicsWorld = nullptr
			);

			/**
			 * @brief �p�X�ړ����I���������H�𓾂�
			 * @return �p�X�ړ����I���������H
			*/
			bool IsEndPathMove() const
			{
				return m_isEndPathMove;
			}

		private:	// private�ȃ����o�֐�

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


		private:	// �f�[�^�����o
			nsAI::CPath m_path;		//!< �p�X
			const nsAI::CNaviMesh* m_naviMeshRef = nullptr;		//!< �i�r���b�V���̎Q��
			nsAI::CPathFinding* m_pathFindingRef = nullptr;		//!< �p�X���������̎Q��
			std::vector<Vector3>* m_naviMeshTargetPointsRef = nullptr;	//!< �i�r���b�V���̃^�[�Q�b�g�̎Q��
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			bool m_isEndPathMove = true;						//!< �p�X�ړ����I���������H
			Vector3 m_moveDir = Vector3::Front;					//!< �ړ�����

			std::unique_ptr<std::mt19937> m_mt;							//!< �����Z���k�c�C�X�^�[��32�r�b�g��
			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< �͈͕t���̈�l����
			//!< ���S�^�]���Ɏg�p����͈͕t���̈�l����
			std::unique_ptr<std::uniform_int_distribution<>> m_randForSafeDrive;
		};
	}
}
