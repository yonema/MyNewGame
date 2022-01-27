#pragma once
#include "UIConstData.h"

// �O���錾
class Level2D;

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// �X�v���C�g�����_���[
		namespace nsFont { class CFontRender; }		// �t�H���g�����_���[
	}


	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �~�b�V������\������UI�N���X
		*/
		class CMissionUI : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CMissionUI() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMissionUI() = default;

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
			 * @brief �~�b�V������\������
			*/
			void ShowMission();

			/**
			 * @brief �~�b�V��������N���A�������̏���
			 * @param[in] missionType �N���A�����~�b�V�����̃^�C�v
			*/
			void ClearOneMission(const nsMissionUIConstData::EnMissionType missionType);

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �X�v���C�g�̏�����
			*/
			void InitSprite();

			/**
			 * @brief �t�H���g�̏�����
			*/
			void InitFont();

			/**
			 * @brief �~�b�V������\�����鏈�����X�V
			*/
			void UpdataShowMission();

			/**
			 * @brief �~�b�V�������\���ɂ��鏈�����X�V
			*/
			void UpdateHideMission();

			/**
			 * @brief �~�b�V��������N���A�������̏����̍X�V
			*/
			void UpdateClearOneMission();

			/**
			 * @brief �~�b�V�����̃X�v���C�g�����_���[�S�ĂɃN�G�����s��
			 * @param[in] func ���s����֐�
			*/
			void QueryAllMissionSRs(const std::function<void(nsGraphic::nsSprite::CSpriteRender* spriteRender)> func);

			/**
			 * @brief �X�e�[�g��J�ڂ���
			 * @param[in] newState �V�����X�e�[�g
			*/
			void ChangeState(nsMissionUIConstData::EnMissionState newState);

		private:	// �f�[�^�����o
			Level2D* m_missionLevel = nullptr;	//!< �~�b�V�����p�̃��x��2D�N���X
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_checkMarkSRs;	//!< �`�F�b�N�}�[�N�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_missionWindowSR = nullptr;	//!< �~�b�V�����E�B���h�E�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_missionResultFrameSR = nullptr;	//!< �~�b�V�������U���g�̘g�̃X�v���C�g�����_���[
			//!< �~�b�V�������U���g�̃e�L�X�g�̃X�v���C�g�����_���[
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_missionResultTextSRs;

			nsGraphic::nsFont::CFontRender* m_clearTimeFR = nullptr;	//!< �N���A�^�C���̃t�H���g�����_���[
			nsGraphic::nsFont::CFontRender* m_numOfMissFR = nullptr;	//!< �~�X�̉񐔂̃t�H���g�����_���[

			std::vector<Vector3> m_checkMarkOffsets;	//!< �`�F�b�N�}�[�N�̃I�t�Z�b�g
			std::vector<bool> m_checkMarkFlag;			//!< �`�F�b�N�}�[�N�̃N���A�t���O
			float m_timer = 0.0f;	//!< �^�C�}�[

			//!< �~�b�V�����X�e�[�g
			nsMissionUIConstData::EnMissionState m_missionState = nsMissionUIConstData::enMS_none;
			//!< ���݂̃N���A�����~�b�V�����̃^�C�v
			nsMissionUIConstData::EnMissionType m_currentClearMissionType = 
				static_cast<nsMissionUIConstData::EnMissionType>(-1);
		};

	}
}