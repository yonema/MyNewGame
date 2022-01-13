#pragma once
#include "../../ExEngine/effect/Effect.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �G�t�F�N�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsEffect
	{
		/**
		 * @brief �G�t�F�N�g�Đ��N���X
		*/
		class CEffectPlayer : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			CEffectPlayer() = default;	// �R���X�g���N�^
			~CEffectPlayer() = default;	// �f�X�g���N�^

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s����
			*/
			bool Start() override final { return true; };

			/**
			 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
			*/
			void AlwaysUpdate() override final;

		public:		// �����o�֐�

			/**
			 * @brief �������֐�
			 * @param[in] filePath �t�@�C���p�X
			*/
			void Init(const char16_t* filePath);

			/**
			 * @brief �G�t�F�N�g���Đ�����
			*/
			void Play();

			/**
			 * @brief �G�t�F�N�g���~����
			*/
			void Stop()
			{
				m_effect.Stop();
			}

			/**
			 * @brief ���W��ݒ�
			 * @param[in] pos ���W
			*/
			void SetPosition(const Vector3& pos)
			{
				m_effect.SetPosition(pos);
			}

			/**
			 * @brief ���W���擾
			 * @return ���W
			*/
			const Vector3& GetPosition() const
			{
				return m_effect.GetPosition();
			}

			/**
			 * @brief ��]��ݒ�
			 * @param[in] rot ��]
			*/
			void SetRotation(const Quaternion& rot)
			{
				m_effect.SetRotation(rot);
			}

			/**
			 * @brief ��]���擾
			 * @return ��]
			*/
			const Quaternion& GetRotation() const
			{
				return m_effect.GetRotation();
			}

			/**
			 * @brief �g�嗦��ݒ�iVector3�j
			 * @param[in] scale �g�嗦
			*/
			void SetScale(const Vector3& scale)
			{
				m_effect.SetScale(scale);
			}

			/**
			 * @brief �g�嗦��ݒ�ifloat�j
			 * @param[in] scale �g�嗦
			*/
			void SetScale(const float scale)
			{
				m_effect.SetScale(scale);
			}

			/**
			 * @brief �g�嗦���擾
			 * @return �g�嗦
			*/
			const Vector3& GetScale() const
			{
				return m_effect.GetScale();
			}

			/**
			 * @brief �G�t�F�N�g���Đ������H
			 * @return �Đ������H
			*/
			bool IsPlay() const
			{
				return m_effect.IsPlay();
			}

		private:	//�f�[�^�����o
			Effect m_effect;	//!< �G�t�F�N�g�N���X
		};
	}
}


