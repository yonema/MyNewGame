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
#include "RenderingEngine.h"


namespace nsNinjaAttract
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief テストマップ（レベル）用のネームスペース
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool CTestMap::Start()
			{
				// アニメーションの初期化
				m_animationClip[enAnim_idle].Load("Assets/animData/idle.tka");
				m_animationClip[enAnim_idle].SetLoopFlag(true);
				m_animationClip[enAnim_walk].Load("Assets/animData/walk.tka");
				m_animationClip[enAnim_walk].SetLoopFlag(true);
				// モデルの初期化
				m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				Quaternion qRot;
				qRot.SetRotationDegY(180.0f);
				m_modelRender->SetRotation(qRot);
				m_modelRender->Init("Assets/modelData/unityChan.tkm", m_animationClip, enAnim_num, enModelUpAxisY);
				m_modelRender->SetIsShadowCaster(true);
				m_modelRender->SetIsShadowReciever(true);

				// フォントの初期化
				m_fontRender = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
				m_fontRender->SetParam(L"バンドリ");
				m_fontRender->SetShadowParam(true, 1.0f, Vector4::Black);
				m_fontRender->SetPosition({ 300.0f, 350.0f });

				// スプライトの初期化
				m_spriteRender = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFirst);
				m_spriteRender->Init("Assets/Image/sample.dds", 256, 256, { 0.0f,1.0f });
				m_spriteRender->SetPosition({ 300.0f,300.0f,0.0f });

				// サウンドキューの初期化
				m_soundCue = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_soundCue->Init(L"Assets/sound/V0032.wav", nsSound::CSoundCue::enSE);

				// エフェクトプレイヤーの初期化
				m_effectPlayer = NewGO<nsEffect::CEffectPlayer>(nsCommonData::enPriorityFirst);
				m_effectPlayer->Init(u"Assets/effect/laser.efk");
				m_effectPlayer->SetScale(3.0f);
				qRot.SetRotationDegX(-90.0f);
				m_effectPlayer->SetRotation(qRot);

				// スカイキューブの初期化
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day,true);
				g_camera3D->SetFar(40000.0f);
				m_skyCube->SetDestroyEvent([&]() {m_skyCube = nullptr; });

				m_lightModel = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPrioritySecond);
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
			 * @brief 破棄した時に呼ばれる
			*/
			void CTestMap::OnDestroy()
			{
				DeleteGO(m_modelRender);
				DeleteGO(m_fontRender);
				DeleteGO(m_spriteRender);
				DeleteGO(m_effectPlayer);
				DeleteGO(m_soundCue);
				DeleteGO(m_skyCube);
				DeleteGO(m_lightModel);
				DeleteGO(m_pointLight);
				QueryGOs<nsLight::CPointLight>(
					m_pointLigName,
					[&](nsLight::CPointLight* pointLig)->bool
					{
						DeleteGO(pointLig);
						return true;
					}
				);
				QueryGOs<nsGraphic::nsModel::CModelRender>(
					m_pointLigModelName,
					[&](nsGraphic::nsModel::CModelRender* pointLigModel)->bool
					{
						DeleteGO(pointLigModel);
						return true;
					}
				);
				return;
			}

			/**
			 * @brief アップデート関数
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
						NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPrioritySecond, m_pointLigModelName);
					model->IniTranslucent("Assets/modelData/debugModel/light.tkm");
					model->SetPosition(m_pointLight->GetPosition());
					model->SetEmmisonColor({ 10.0f,0.0f,0.0f,1.0f });
					model->SetAlphaValue(0.5f);

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

				}

				if (g_pad[0]->IsTrigger(enButtonX))
				{
					if (m_skyCube)
					{
						DeleteGO(m_skyCube);
						nsMyEngine::CRenderingEngine::GetInstance()->SetIsIBL(false);
					}
					else
					{
						// スカイキューブの初期化
						m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
						static int skyCubeType = nsNature::nsSkyCubeConstData::enSkyCubeType_day;
						if (skyCubeType != nsNature::nsSkyCubeConstData::enSkyCubeType_snow)
						{
							skyCubeType++;
						}
						else
						{
							skyCubeType = nsNature::nsSkyCubeConstData::enSkyCubeType_day;
						}
						m_skyCube->Init(static_cast<nsNature::nsSkyCubeConstData::EnSkyCubeType>(skyCubeType), true);
						g_camera3D->SetFar(40000.0f);
						m_skyCube->SetDestroyEvent([&]() {m_skyCube = nullptr; });
					}
				}

				m_modelRender->SetPosition(m_modelRender->GetPosition() + moveVec);
				m_lightModel->SetPosition(m_lightModel->GetPosition() + ligMoveVec);
				m_pointLight->SetPosition(m_lightModel->GetPosition());

				return;
			}
		}
	}
}