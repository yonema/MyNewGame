#include "stdafx.h"
#include "Fade.h"
#include "SpriteRender.h"
#include "GameTime.h"

namespace nsNinjaAttract
{

	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		const char* const CFade::kFadeSpriteFilePath = "Assets/Image/black.dds";
		const float CFade::kDefaultFadeTime = 2.0f;

		/**
		 * @brief コンストラクタ
		*/
		CFade::CFade()
		{
			// フェードのスプライトレンダラーのオブジェクトを生成する
			m_fadeSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
		}

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CFade::Start()
		{

			// フェード用のスプライトの初期化
			m_fadeSR->Init(
				kFadeSpriteFilePath,
				static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()),
				static_cast<float>(g_graphicsEngine->GetFrameBufferHeight()),
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			if (m_fadeState == enFS_blackOut)
			{
				// ブラックアウト状態なら、アルファ値を1にする。
				m_fadeSR->SetAlphaValue(1.0f);
			}
			else
			{
				// 通常状態なら、アルファ値を0にする。
				m_fadeSR->SetAlphaValue(0.0f);
			}

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CFade::OnDestroy()
		{
			DeleteGO(m_fadeSR);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CFade::Update()
		{
			if (m_fadeState == enFS_fadeEnd)
			{
				// フェードが終わっていたら、何もしない。早期リターン。
				return;
			}

			// フェードのタイマーを進める
			m_fadeTimer += nsTimer::GameTime().GetFrameDeltaTime();
			// フェードタイマーの進み率
			const float scale = min(1.0f, m_fadeTimer / m_fadeTime);
			// アルファ値
			float alphaValue = 0.0f;

			if (m_fadeState == enFS_fadeIn)
			{
				// フェードインなら、アルファ値をだんだん小さくしていく。
				alphaValue = 1.0f - 1.0f * scale;
			}
			else if (m_fadeState == enFS_fadeOut)
			{
				// フェードアウトなら、アルファ値をだんだん大きくしていく。
				alphaValue = 1.0f * scale;
			}
			else
			{
				// ブラックアウトなら、アルファ値を1にしてフェードを終了する。
				alphaValue = 1.0f;
				m_fadeState = enFS_fadeEnd;
			}

			// アルファ値を設定する
			m_fadeSR->SetAlphaValue(alphaValue);

			if (scale >= 1.0f)
			{
				// タイマーの進み率が1を超えたらフェードを終了する
				m_fadeState = enFS_fadeEnd;
			}

			return;
		}

	}
}