#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "SoundCue.h"
#include "EffectPlayer.h"


namespace nsMyGame
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
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
			m_modelRender->Init("Assets/modelData/unityChan.tkm",enModelUpAxisY, m_animationClip,enAnim_num);
			Quaternion qRot;
			qRot.SetRotationDegY(180.0f);
			m_modelRender->SetRotatioin(qRot);

			// フォントの初期化
			m_fontRender = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityThird);
			m_fontRender->SetParam(L"バンドリ");
			m_fontRender->SetShadowParam(true, 1.0f, Vector4::Black);
			m_fontRender->SetPosition({ 300.0f, 350.0f });

			// スプライトの初期化
			m_spriteRender = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_spriteRender->Init("Assets/Image/sample.dds", 256, 256, {0.0f,1.0f});
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
			return;
		}

		/**
		 * @brief アップデート関数
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
				m_effectPlayer->SetPosition(m_modelRender->GetPosition());
				m_effectPlayer->Play();
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