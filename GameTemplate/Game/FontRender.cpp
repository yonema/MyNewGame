#include "stdafx.h"
#include "FontRender.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief フォント関連のネームスペース
		*/
		namespace nsFont
		{
			/**
			 * @brief 破棄されたときの処理
			*/
			void CFontRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief 描画処理関数
			 * @param[in] rc レンダーコンテキスト
			*/
			void CFontRender::Render(RenderContext& rc)
			{
				//描画開始
				m_font.Begin(rc);

				//描画処理
				m_font.Draw(
					m_text,
					m_position,
					m_color,
					m_rotation,
					m_scale,
					m_pivot);

				//描画終了
				m_font.End(rc);

				return;
			}

			/**
			 * @brief フォントのパラメータを設定
			 * @param[in] text テキスト
			 * @param[in] pos 座標
			 * @param[in] color カラー
			 * @param[in] rotation 回転
			 * @param[in] scale 拡大率
			 * @param[in] pivot ピボット
			*/
			void CFontRender::SetParam(
				const wchar_t* text,
				const Vector2& pos,
				const Vector4& color,
				const float rotation,
				const float scale,
				const Vector2& pivot
			)
			{
				//各フォントのパラメータを設定する

				SetText(text);
				SetPosition(pos);
				m_color = color;
				m_rotation = rotation;
				m_scale = scale;
				m_pivot = pivot;

				return;
			}
		}
	}
}