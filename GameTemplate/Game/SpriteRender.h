#pragma once
#include "SpriteRenderConstData.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �X�v���C�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsSprite
		{
			/**
			 * @brief �X�v���C�g�`��N���X
			*/
			class CSpriteRender : public IGameObject
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CSpriteRender() = default;

				/**
				 * @brief �f�X�g���N�^
				*/
				~CSpriteRender() = default;

			public:		// �I�[�o�[���C�h���������o�֐�
				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final { return true; };

				/**
				 * @brief �j�����ꂽ�Ƃ��̏���
				*/
				void OnDestroy() override final;

				/**
				 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
				*/
				void AlwaysUpdate() override final;

				/**
				 * @brief �`�揈���֐�
				 * @param[in] rc �����_�[�R���e�L�X�g
				*/
				void Render(RenderContext & rc) override final;

			public:		// �����o�֐�
				/**
				 * @brief �������p�֐��B���̈�����<float>�o�[�W�����B�ŏ��ɌĂ�łˁB
				 * @attention ����UINT�ɃL���X�g�����̂ŁA�����_�ȉ��̃f�[�^�͎�����B
				 * @param[in] texFilePath dds�t�@�C���p�X
				 * @param[in] w �X�v���C�g�̉���
				 * @param[in] h �X�v���C�g�̏c��
				 * @param[in] pivot �s�{�b�g�i��_�j
				 * @param[in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
				*/
				void Init(
					const char* texFilePath,
					const float w,
					const float h,
					const Vector2& pivot = nsSpriteConstData::kDefaultPivot,
					const AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
				);

				/**
				 * @brief �������p�֐��B���̈�����<int>�o�[�W�����B�ŏ��ɌĂ�łˁB
				 * @param[in] texFilePath dds�t�@�C���p�X
				 * @param[in] w �X�v���C�g�̉���
				 * @param[in] h �X�v���C�g�̏c��
				 * @param[in] pivot �s�{�b�g�i��_�j
				 * @param[in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
				*/
				void Init(
					const char* texFilePath,
					const int w,
					const int h,
					const Vector2& pivot = nsSpriteConstData::kDefaultPivot,
					const AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
				);

				/**
				 * @brief ���W��ݒ�
				 * @param[in] pos ���W
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief ���W���擾
				 * @return ���W
				*/
				const Vector3& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief  �g�嗦��ݒ�
				 * @param[in] scale �g�嗦
				*/
				void SetScale(const Vector3& scale)
				{
					m_scale = scale;
				}

				/**
				 * @brief �g�嗦���擾
				 * @return �g�嗦
				*/
				const Vector3& GetScale() const
				{
					return m_scale;
				}

				/**
				 * @brief ��]��ݒ�
				 * @param[in] rot ��]
				*/
				void SetRotation(const Quaternion& rot)
				{
					m_rotation = rot;
				}

				/**
				 * @brief ��]���擾
				 * @return ��]
				*/
				const Quaternion& GetRotation() const
				{
					return m_rotation;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �������p�֐��̃��C���R�A�BInit�֐��̒��ŌĂ΂��B
				 * @param[in] texFilePath dds�t�@�C���p�X
				 * @param[in] w �X�v���C�g�̉���
				 * @param[in] h �X�v���C�g�̏c��
				 * @param[in] pivot �s�{�b�g�i��_�j
				 * @param[in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
				*/
				void InitMainCore(
					const char* texFilePath,
					const UINT w,
					const UINT h,
					const Vector2& pivot,
					const AlphaBlendMode alphaBlendMode
				);

			private:	// �f�[�^�����o
				Sprite m_sprite;		//!< �X�v���C�g�N���X
				Vector3 m_position = Vector3::Zero;
				Quaternion m_rotation = Quaternion::Identity;
				Vector3 m_scale = Vector3::One;
				Vector2 m_pivot = nsSpriteConstData::kDefaultPivot;


			};
		}
	}
}