#include "stdafx.h"
#include "SpriteRender.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
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
			 * @brief �R���X�g���N�^
			*/
			CSpriteRender::CSpriteRender()
			{
				m_render.SetOnRender2DFunc([&](RenderContext& rc) {this->Render2D(rc); });

				return;
			}

			/**
			 * @brief �j�����ꂽ�Ƃ��̏���
			*/
			void CSpriteRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
			*/
			void CSpriteRender::AlwaysUpdate()
			{
				//�X�v���C�g�̍X�V
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
				return;
			}

			/**
			 * @brief �`��I�u�W�F�N�g�o�^�̓���
			*/
			void CSpriteRender::AddRenderEntrance()
			{
				// �`��I�u�W�F�N�g�ɓo�^����
				nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);

				return;
			}

			/**
			 * @brief 2D�`��֐�
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void CSpriteRender::Render2D(RenderContext& rc)
			{
				//�X�v���C�g�̕`��
				m_sprite.Draw(rc);

				return;
			}

			/**
			 * @brief �������p�֐��B���̈�����<float>�o�[�W�����B�ŏ��ɌĂ�łˁB
			 * @attention ����UINT�ɃL���X�g�����̂ŁA�����_�ȉ��̃f�[�^�͎�����B
			 * @param [in] texFilePath dds�t�@�C���p�X
			 * @param [in] w �X�v���C�g�̉���
			 * @param [in] h �X�v���C�g�̏c��
			 * @param [in] pivot �s�{�b�g�i��_�j
			 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
			*/
			void CSpriteRender::Init(
				const char* texFilePath,
				const float w,
				const float h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				//�������p�֐��̃��C���R�A
				InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

				return;
			}

			/**
			 * @brief �������p�֐��B���̈�����<int>�o�[�W�����B�ŏ��ɌĂ�łˁB
			 * @param [in] texFilePath dds�t�@�C���p�X
			 * @param [in] w �X�v���C�g�̉���
			 * @param [in] h �X�v���C�g�̏c��
			 * @param [in] pivot �s�{�b�g�i��_�j
			 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
			*/
			void CSpriteRender::Init(
				const char* texFilePath,
				const int w,
				const int h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				//�������p�֐��̃��C���R�A
				//int��float�ɃL���X�g���Ă���n��
				InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

				return;
			}

			/**
			 * @brief �������f�[�^�ɂ�鏉���������B�ŏ��ɌĂ�łˁB
			 * @param[in] spriteInitData �X�v���C�g�̏������f�[�^
			*/
			void CSpriteRender::Init(const SpriteInitData& spriteInitData)
			{
				//�X�v���C�g�̏�����
				m_sprite.Init(spriteInitData);

				//�X�v���C�g�̍X�V
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

				return;
			}

			/**
			 * @brief �������p�֐��̃��C���R�A�BInit�֐��̒��ŌĂ΂��B
			 * @param [in] texFilePath dds�t�@�C���p�X
			 * @param [in] w �X�v���C�g�̉���
			 * @param [in] h �X�v���C�g�̏c��
			 * @param [in] pivot �s�{�b�g�i��_�j
			 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
			*/
			void CSpriteRender::InitMainCore(
				const char* texFilePath,
				const UINT w,
				const UINT h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				using namespace nsSpriteConstData;
				//�X�v���C�g�̏������p�f�[�^
				SpriteInitData initData;
				initData.m_ddsFilePath[0] = texFilePath;
				initData.m_width = w;
				initData.m_height = h;
				initData.m_fxFilePath = kDefaultFxFilePath;
				initData.m_psEntryPoinFunc = kPsEntryPointFunc;
				initData.m_vsEntryPointFunc = kVsEntryPointFunc;
				initData.m_alphaBlendMode = alphaBlendMode;
				// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[�t�H�[�}�b�g�ƍ��킹��
				initData.m_colorBufferFormat[0] = nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

				m_pivot = pivot;

				//�X�v���C�g�̏�����
				m_sprite.Init(initData);

				//�X�v���C�g�̍X�V
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

				return;
			}

		}
	}
}