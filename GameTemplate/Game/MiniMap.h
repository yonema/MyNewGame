#pragma once

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// �X�v���C�g�����_���[
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }

	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �~�j�}�b�v�\���N���X
		*/
		class CMiniMap : public IGameObject
		{
		private:	// �\����
			/**
			 * @brief �~�j�}�b�v�̃X�v���C�g�p�̒萔�o�b�t�@
			*/
			struct SMiniMapSpriteCB
			{
				Vector2 minPos = Vector2::Zero;		//!< �\������ŏ����W
				Vector2 maxPos = Vector2::Zero;		//!< �\������ő���W
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CMiniMap() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMiniMap() = default;

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
			
		private:	// private�ȃ����o�֐�

			/**
			 * @brief �~�j�}�b�v�̃X�v���C�g�̏�����
			*/
			void InitMiniMapSprite();

			/**
			 * @brief �v���C���[�̃A�C�R���̃X�v���C�g�̏�����
			*/
			void InitPlayerIconSprite();

			/**
			 * @brief �Ԃ̃A�C�R���̃X�v���C�g�̏�����
			*/
			void InitCarIconSprite();

			/**
			 * @brief �Ԃ̃~�j�A�C�R���̃X�v���C�g�̏�����
			*/
			void InitCarMiniIconSprite();

			/**
			 * @brief �~�j�}�b�v�̍ŏ����W�ƍő���W���v�Z
			*/
			void CalcMinAndMaxPos();

			/**
			 * @brief �N���X���̂����ȂƂ���ŋ��ʂ��Ďg�p�����f�[�^�̍X�V
			*/
			void UpdateCommonData();

			/**
			 * @brief �~�j�}�b�v�̊�_���X�V
			*/
			void UpdateMiniMapPivot();

			/**
			 * @brief �~�j�}�b�v�̉�]���X�V
			*/
			void UpdateMiniMapRotate();

			/**
			 * @brief �v���C���[�̃A�C�R������]���X�V
			*/
			void UpdatePlayerIconRotate();

			/**
			 * @brief �Ԃ̃A�C�R�����X�V
			*/
			void UpdateCarIcon();

			/**
			 * @brief �Ԃ̃~�j�A�C�R�����X�V
			*/
			void UpdateCarMiniIcon();

			/**
			 * @brief �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p���X�V�B
			 * ���̊֐��́AUpdateCarMiniIcon�ŌĂ΂��B
			 * @param[in] index �Ԃ̃C���f�b�N�X
			 * @param[in] isIntersect �������Ă��邩�H
			 * @param[in] playerToCarVec �v���C���[����Ԃւ̃x�N�g��
			 * @param[in] iconPos �Ԃ̃A�C�R���̍��W
			*/
			void UpdateCarMiniIconOut(
				const int index,
				const bool isIntersect,
				const Vector3& playerToCarVec,
				const Vector2& iconPos
			);

		private:	// �f�[�^�����o
			//!< �~�j�}�b�v�̔w�i�p�X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_miniMapBackSR = nullptr;
			//!< �~�j�}�b�v�̘g�p�X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_miniMapFrameSR = nullptr;
			//!< �~�j�}�b�v�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_miniMapSR = nullptr;
			//!< �v���C���[�̃A�C�R���̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_playerIconSR = nullptr;
			//!< �Ԃ̃A�C�R���̃X�v���C�g�����_���[
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carIconSRs;
			//!< �Ԃ̃~�j�A�C�R���̃X�v���C�g�����_���[
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carMiniIconSRs;
			//!< �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p�X�v���C�g�����_���[
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carMiniIconOutSRs;

			//////// ���ʃp�����[�^ ////////
			const Vector3* m_playerPositionRef = nullptr;			//!< �v���C���[�̍��W�̎Q��
			std::vector<const nsAICharacter::CAICar*> m_aiCarsRef;	//!< �Ԃ�����const�Q��
			Vector3 m_cameraForwardXZ = Vector3::Front;				//!< XZ���ʂł̃J�����̑O����
			Vector3 m_cameraRightXZ = Vector3::Right;				//!< XZ���ʂł̃J�����̑O����
			Quaternion m_miniMapRotatioin = Quaternion::Identity;	//!< �~�j�}�b�v�̉�]

			SMiniMapSpriteCB m_miniMapSpriteCB;					//!< �~�j�}�b�v�p�̒萔�o�b�t�@
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g
		};

	}
}