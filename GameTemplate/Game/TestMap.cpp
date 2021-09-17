#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "SoundCue.h"

namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �X�^�[�g�֐�
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CTestMap::Start()
		{
			// �A�j���[�V�����̏�����
			m_animationClip[enAnim_idle].Load("Assets/animData/idle.tka");
			m_animationClip[enAnim_idle].SetLoopFlag(true);
			m_animationClip[enAnim_walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_walk].SetLoopFlag(true);
			// ���f���̏�����
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init("Assets/modelData/unityChan.tkm",enModelUpAxisY, m_animationClip,enAnim_num);
			Quaternion qRot;
			qRot.SetRotationDegY(180.0f);
			m_modelRender->SetRotatioin(qRot);

			// �t�H���g�̏�����
			m_fontRender = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityThird);
			m_fontRender->SetParam(L"�o���h��");
			m_fontRender->SetShadowParam(true, 1.0f, Vector4::Black);
			m_fontRender->SetPosition({ 300.0f, 350.0f });

			// �X�v���C�g�̏�����
			m_spriteRender = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_spriteRender->Init("Assets/Image/sample.dds", 256, 256, {0.0f,1.0f});
			m_spriteRender->SetPosition({ 300.0f,300.0f,0.0f });

			//�T�E���h�L���[�̏�����
			m_soundCue = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_soundCue->Init(L"Assets/sound/V0032.wav", nsSound::CSoundCue::enSE);

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��
		*/
		void CTestMap::OnDestroy()
		{
			DeleteGO(m_modelRender);
			DeleteGO(m_fontRender);
			DeleteGO(m_spriteRender);
			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CTestMap::Update()
		{
			Vector3 moveVec = Vector3::Zero;
			const float moveSpeed = 1.0f;

			if (g_pad[0]->GetLStickXF() >= 0.5f)
			{
				moveVec.x += moveSpeed;
			}
			else if (g_pad[0]->GetLStickXF() <= -0.5f)
			{
				moveVec.x -= moveSpeed;
			}

			if (g_pad[0]->GetLStickYF() >= 0.5f)
			{
				moveVec.z += moveSpeed;
			}
			else if (g_pad[0]->GetLStickYF() <= -0.5f)
			{
				moveVec.z -= moveSpeed;
			}

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_soundCue->Play(false);
				if (m_animState == enAnim_idle)
				{
					m_animState = enAnim_walk;
					m_modelRender->PlayAnimation(enAnim_walk);
				}
				else
				{
					m_animState = enAnim_idle;
					m_modelRender->PlayAnimation(enAnim_idle);
				}
			}

			m_modelRender->SetPosition(m_modelRender->GetPosition() + moveVec);

			return;
		}
	}
}