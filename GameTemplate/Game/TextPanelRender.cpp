#include "stdafx.h"
#include "TextPanelRender.h"
#include "RenderingEngine.h"

namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{

		// デバックの定数データを使用可能にする
		using namespace nsDebugConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CTextPanelRender::Start()
		{
			// テキストパネルのデータ集のキャパシティを確保
			m_textPanelDatas.reserve(kTextPanelDatasNum);

			// レンダラークラスに2D描画関数を設定する
			m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

			// フォントのシャドウパラメータを設定
			m_font.SetShadowParam(true, m_shadowOffset, Vector4::Black);

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CTextPanelRender::OnDestroy()
		{
			return;
		}

		/**
		 * @brief 描画オブジェクト登録の入口
		*/
		void CTextPanelRender::AddRenderEntrance()
		{
			// 描画オブジェクトに登録する
			nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);
		}


		/**
		 * @brief 2D描画関数
		 * @param[in,out] rc レンダリングコンテキスト
		*/
		void CTextPanelRender::Render2D(RenderContext& rc)
		{
			m_font.Begin(rc);

			Vector2 pos = m_textPanelPosition;

			for (const auto& textPanelData : m_textPanelDatas)
			{
				// テキスト
				wchar_t text[kTextSize];
				// charをwcharに変換
				mbstowcs(text, textPanelData, kTextSize);
				// テキストの縦の間
				const float between = 40.0f * m_textScale;
				// 基点
				const Vector2 pivot = { 0.0f,1.0f };

				// テキストを描画
				m_font.Draw(text, pos, Vector4::White, 0.0f, m_textScale, pivot);

				// 座標を下に下げる	
				pos.y -= between;

			}



			// フォントの描画終了
			m_font.End(rc);

			// ベクトルのデータ集をクリアする
			m_textPanelDatas.clear();

			return;
		}

	}
}