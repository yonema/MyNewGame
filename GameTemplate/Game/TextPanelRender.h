#pragma once
#include "Render.h"
#include "DebugConstData.h"

namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{
		/**
		 * @brief デバック用のテキストパネル描画クラス
		*/
		class CTextPanelRender : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CTextPanelRender() = default;
			/**
			 * @brief デストラクタ
			*/
			~CTextPanelRender() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 描画オブジェクト登録の入口
			*/
			void AddRenderEntrance() override final;

		public:		// メンバ関数

			/**
			 * @brief 描画するテキストをテキストパネルに追加
			 * @param[in] text テキスト
			 * @param[in] headText 先頭に付けるテキスト
			*/
			void AddTextPanel(const std::wstring& text, const std::wstring& headText)
			{
				m_textPanelDatas.emplace_back(std::move(headText + text));
			}

			/**
			 * @brief 2D描画関数
			 * @param[in,out] rc レンダリングコンテキスト
			*/
			void Render2D(RenderContext& rc);

		private:	// データメンバ
			std::vector<std::wstring> m_textPanelDatas;						//!< テキストパネルのデータ集
			nsGraphic::CRender m_render;									//!< レンダラークラス
			float m_textScale = nsDebugConstData::kDefaultTextScale;		//!< テキストの拡大率
			Font m_font;													//!< フォント
			Vector2 m_textPanelPosition = nsDebugConstData::kDefaultTextPosition;	//!< テキストパネルの座標
			bool m_isShadow = false;										//!< シャドウを描画するか？
			float m_shadowOffset = nsDebugConstData::kDefaultTextShadowOffset;	//!< シャドウのオフセット
		};

	}
}