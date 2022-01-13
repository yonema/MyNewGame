#pragma once
#include "BGMConstData.h"

namespace nsNinjaAttract
{
	namespace nsPlayer { class CPlayer; }
	namespace nsSound { class CSoundCue; }

	namespace nsBGM
	{
		class CBGM : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBGM() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBGM() = default;

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
			 * @brief ������
			 * @param[in] player �v���C���[�̎Q��
			*/
			void Init(const nsPlayer::CPlayer& player);

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �N���X�t�F�[�h��ݒ肷��
			 * @param[in] bgmType ����BGM�̎��
			*/
			void SetCrossFade(const nsBGMConstData::EnBGMType bgmType);

			/**
			 * @brief BGM�̃X�e�[�g���X�V����
			*/
			void UpdateBGMState();

			/**
			 * @brief �N���X�t�F�[�h���X�V
			*/
			void UpdateCrossFade();

			/**
			 * @brief �X�e�[�g�J��
			 * @param[in] newState �V�����X�e�[�g
			*/
			void ChangeState(const nsBGMConstData::EnBGMType newState);

		private:	// �f�[�^�����o
			const nsPlayer::CPlayer* m_playerRef = nullptr;		//!< �v���C���[�N���X�̎Q��

			//!< �X�C���O����BGM�̃T�E���h
			nsSound::CSoundCue* m_bgmSC[nsBGMConstData::enBGMTypeNum] = {};

			//!< ���݂�BGM�̎��
			nsBGMConstData::EnBGMType m_currentBGMType = nsBGMConstData::enBT_Normal;
			//!< ����BGM�̎��
			nsBGMConstData::EnBGMType m_nextBGMType = nsBGMConstData::enBT_Normal;
			//!< BGM�̃X�e�[�g
			nsBGMConstData::EnBGMType m_bgmState = nsBGMConstData::enBT_Normal;
			bool m_isCrossFade = false;					//!< �N���X�t�F�[�h�����H

			float m_crossFadeTimer = 0.0f;				//!< �N���X�t�F�[�h�Ɏg�p����^�C�}�[
			float m_crossFadeTime = 0.0f;				//!< �N���X�t�F�[�h�̃^�C��
		};

	}
}