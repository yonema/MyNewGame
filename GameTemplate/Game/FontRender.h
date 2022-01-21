#pragma once
#include "FontRenderConstData.h"
#include "Render.h"

namespace nsNinjaAttract
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
			 * @brief フォント描画クラス
			*/
			class CFontRender : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CFontRender();

				/**
				 * @brief デストラクタ
				*/
				~CFontRender() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final { return true; };

				/**
				 * @brief 破棄されたときの処理
				*/
				void OnDestroy() override final;

				/**
				 * @brief 描画オブジェクト登録の入口
				*/
				void AddRenderEntrance() override final;

			public:		// メンバ関数

				/**
				 * @brief フォントのパラメータを設定
				 * @param[in] text テキスト
				 * @param[in] pos 座標
				 * @param[in] color カラー
				 * @param[in] rotation 回転
				 * @param[in] scale 拡大率
				 * @param[in] pivot ピボット
				*/
				void SetParam(
					const wchar_t* text,
					const Vector2& pos = Vector2::Zero,
					const Vector4& color = nsFontConstData::kDefaltTextColor,
					const float rotation = 0.0f,
					const float scale = 1.0f,
					const Vector2& pivot = Vector2::Zero
				);

				/**
				 * @brief 座標を設定
				 * @param[on] pos 
				*/
				void SetPosition(const Vector2& pos)
				{
					m_position = pos;
				}
				

				/**
				 * @brief 座標を取得
				 * @return 座標
				*/
				const Vector2& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief テキストを設定
				 * @param[in] text テキスト
				*/
				void SetText(const wchar_t* text)
				{
					swprintf_s(m_text, text);
				}

				/**
				 * @brief フォントのシャドウの設定
				 * @param isDrawShadow シャドウを描画するか？
				 * @param shadowOffset シャドウのピクセルのオフセット
				 * @param shadowColor シャドウのカラー
				*/
				void SetShadowParam(
					const bool isDrawShadow,
					const float shadowOffset,
					const Vector4& shadowColor
				)
				{
					m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
				}

				/**
				 * @brief ピボットを設定する
				 * @param[in] pivot ピボット
				*/
				void SetPivot(const Vector2& pivot)
				{
					m_pivot = pivot;
				}

			private:	// privateなメンバ関数
				/**
				 * @brief 2D描画関数
				 * @param rc レンダリングコンテキスト
				*/
				void Render2D(RenderContext & rc);

			private:	// データメンバ
				Font m_font;											//!< フォントクラス
				CRender m_render;										//!< レンダラークラス
				wchar_t m_text[nsFontConstData::kTextMaxSize] = {};		//!< 表示するテキスト
				Vector2 m_position = Vector2::Zero;						//!< 座標
				Vector4 m_color = nsFontConstData::kDefaltTextColor;	//!< カラー
				float m_rotation = 0.0f;								//!< 回転
				float m_scale = 1.0f;									//!< 拡大率
				Vector2 m_pivot = Vector2::Zero;						//!< ピボット
			};

		}
	}
}