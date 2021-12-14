#pragma once
#include "Path.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}
	namespace nsAI {
		class CNaviMesh;
		class CPathFinding;
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
			struct SAICharacterInitData
			{
				const nsAI::CNaviMesh* naviMeshRef = nullptr;
				nsAI::CPathFinding* pathFindingRef = nullptr;
				std::vector<Vector3>* naviMeshTargetPointsRef = nullptr;
			};
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
			void Init(SAICharacterInitData& AICharaInitData)
			{
				m_naviMeshRef = AICharaInitData.naviMeshRef;
				m_pathFindingRef = AICharaInitData.pathFindingRef;
				m_naviMeshTargetPointsRef = AICharaInitData.naviMeshTargetPointsRef;
			}

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

		private:	// �f�[�^�����o
			nsAI::CPath m_path;		//!< �p�X
			const nsAI::CNaviMesh* m_naviMeshRef = nullptr;		//!< �i�r���b�V���̎Q��
			nsAI::CPathFinding* m_pathFindingRef = nullptr;		//!< �p�X���������̎Q��
			std::vector<Vector3>* m_naviMeshTargetPointsRef = nullptr;	//!< �i�r���b�V���̃^�[�Q�b�g�̎Q��
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			bool m_isEndPathMove = true;						//!< �p�X�ړ����I���������H

			std::unique_ptr<std::mt19937> m_mt;							//!< �����Z���k�c�C�X�^�[��32�r�b�g��
			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< �͈͕t���̈�l����
		};
	}
}
