#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "SoundCue.h"
#include "EffectPlayer.h"
#include "LightManager.h"
#include "PointLight.h"
#include "SkyCube.h"


namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �e�X�g�}�b�v�i���x���j�p�̃l�[���X�y�[�X
		*/
		namespace nsTestMaps
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
				m_modelRender->Init("Assets/modelData/unityChan.tkm", enModelUpAxisY, m_animationClip, enAnim_num);
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
				m_spriteRender->Init("Assets/Image/sample.dds", 256, 256, { 0.0f,1.0f });
				m_spriteRender->SetPosition({ 300.0f,300.0f,0.0f });

				// �T�E���h�L���[�̏�����
				m_soundCue = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_soundCue->Init(L"Assets/sound/V0032.wav", nsSound::CSoundCue::enSE);

				// �G�t�F�N�g�v���C���[�̏�����
				m_effectPlayer = NewGO<nsEffect::CEffectPlayer>(nsCommonData::enPriorityFirst);
				m_effectPlayer->Init(u"Assets/effect/laser.efk");
				m_effectPlayer->SetScale(3.0f);
				qRot.SetRotationDegX(-90.0f);
				m_effectPlayer->SetRotation(qRot);

				// �X�J�C�L���[�u�̏�����
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_snow);
				g_camera3D->SetFar(40000.0f);
				m_skyCube->SetDestroyEvent([&]() {m_skyCube = nullptr; });

				m_lightModel = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				m_lightModel->IniTranslucent("Assets/modelData/debugModel/light.tkm");
				m_lightModel->SetPosition({ 0.0f,50.0f,0.0f });
				m_lightModel->SetEmmisonColor({ 10.0f,0.0f,0.0f,1.0f });
				m_lightModel->SetAlphaValue(0.5f);
				m_pointLight = NewGO<nsLight::CPointLight>(nsCommonData::enPriorityFirst);
				m_pointLight->SetPosition({ 0.0f,50.0f,0.0f });
				m_pointLight->SetColor(m_pointLigColor);

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
				DeleteGO(m_skyCube);
				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CTestMap::Update()
			{
				Vector3 moveVec = Vector3::Zero;
				const float moveSpeed = 1.0f;

				Vector3 ligMoveVec = Vector3::Zero;

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

				if (g_pad[0]->GetRStickXF() >= 0.5f)
				{
					ligMoveVec.x += moveSpeed;
				}
				else if (g_pad[0]->GetRStickXF() <= -0.5f)
				{
					ligMoveVec.x -= moveSpeed;
				}
				if (g_pad[0]->GetRStickYF() >= 0.5f)
				{
					ligMoveVec.y += moveSpeed;
				}
				else if (g_pad[0]->GetRStickYF() <= -0.5f)
				{
					ligMoveVec.y -= moveSpeed;
				}


				if (g_pad[0]->IsTrigger(enButtonA))
				{
					m_soundCue->Play(false);
					m_effectPlayer->SetPosition(m_modelRender->GetPosition());
					m_effectPlayer->Play();

					nsLight::CPointLight* pointLig =
						NewGO<nsLight::CPointLight>(nsCommonData::enPriorityFirst, m_pointLigName);
					pointLig->SetColor(m_pointLigColor);
					pointLig->SetPosition(m_pointLight->GetPosition());
					nsGraphic::nsModel::CModelRender* model =
						NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst, m_pointLigModelName);
					model->Init("Assets/modelData/debugModel/light.tkm");
					model->SetPosition(m_pointLight->GetPosition());

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
				else if (g_pad[0]->IsTrigger(enButtonB))
				{
					DeleteGO(FindGO<nsLight::CPointLight>(m_pointLigName));
					DeleteGO(FindGO<nsGraphic::nsModel::CModelRender>(m_pointLigModelName));

					if (m_soundCue->IsPlaying())
						m_soundCue->Pause();
					if (m_skyCube)
						DeleteGO(m_skyCube);
				}

				m_modelRender->SetPosition(m_modelRender->GetPosition() + moveVec);
				m_lightModel->SetPosition(m_lightModel->GetPosition() + ligMoveVec);
				m_pointLight->SetPosition(m_lightModel->GetPosition());

				return;
			}
		}
	}
}