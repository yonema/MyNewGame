#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"
#include "FontRender.h"

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
			m_animationClip[enAnim_idle].Load("Assets/animData/idle.tka");
			m_animationClip[enAnim_idle].SetLoopFlag(true);
			m_animationClip[enAnim_walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_walk].SetLoopFlag(true);
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init("Assets/modelData/unityChan.tkm",enModelUpAxisY, m_animationClip,enAnim_num);
			Quaternion qRot;
			qRot.SetRotationDegY(180.0f);
			m_modelRender->SetRotatioin(qRot);

			m_fontRender = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityThird);
			m_fontRender->SetParam(L"�o���h��");
			m_fontRender->SetShadowParam(true, 1.0f, Vector4::Black);

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��
		*/
		void CTestMap::OnDestroy()
		{
			DeleteGO(m_modelRender);
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