#pragma once
#include "SpriteRenderConstData.h"
#include "Render.h"

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
				CSpriteRender();

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
				 * @brief �`��I�u�W�F�N�g�o�^�̓���
				*/
				void AddRenderEntrance() override final;

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
				 * @brief �������f�[�^�ɂ�鏉���������B�ŏ��ɌĂ�łˁB
				 * @param[in] spriteInitData �X�v���C�g�̏������f�[�^
				*/
				void Init(const SpriteInitData& spriteInitData);

				/**
				 * @brief ���W��ݒ�
				 * @param[in] pos ���W
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief ���W��ݒ�iVector2�j
				 * @param[in] pos ���W
				*/
				void SetPosition(const Vector2& pos)
				{
					m_position.x = pos.x;
					m_position.y = pos.y;
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

				/**
				 * @brief ��Z�J���[��ݒ肷��
				 * @param[in] mulColor ��Z�J���[
				*/
				void SetMulColor(const Vector4& mulColor)
				{
					m_sprite.SetMulColor(mulColor);
				}

				/**
				 * @brief ��Z�J���[�𓾂�
				 * @return ��Z�J���[
				*/
				const Vector4& GetMulColor() const
				{
					return m_sprite.GetMulColor();
				}

				/**
				 * @brief �X�v���C�g�̃A���t�@�l��ݒ肷��
				 * @param[in] alphaValue �A���t�@�l
				*/
				void SetAlphaValue(const float alphaValue)
				{
					m_sprite.SetAlphaValue(alphaValue);
				}

				/**
				 * @brief ��_��ݒ�
				 * @param[in] pivot ��_
				*/
				void SetPivot(const Vector2& pivot)
				{
					m_pivot = pivot;
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

				/**
				 * @brief 2D�`��֐�
				 * @param rc �����_�����O�R���e�L�X�g
				*/
				void Render2D(RenderContext& rc);

			private:	// �f�[�^�����o
				Sprite m_sprite;		//!< �X�v���C�g�N���X
				CRender m_render;		//!< �����_���[�N���X
				Vector3 m_position = Vector3::Zero;						//!< ���W
				Quaternion m_rotation = Quaternion::Identity;			//!< ��]
				Vector3 m_scale = Vector3::One;							//!< �g�嗦
				Vector2 m_pivot = nsSpriteConstData::kDefaultPivot;		//!< �s�{�b�g


			};
		}
	}
}
