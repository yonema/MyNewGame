#pragma once

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラー
	}
	namespace nsGameState { class CGameMainState; }

	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		/**
		 * @brief ミニマップ表示クラス
		*/
		class CMiniMap : public IGameObject
		{
		private:	// 構造体
			/**
			 * @brief ミニマップのスプライト用の定数バッファ
			*/
			struct SMiniMapSpriteCB
			{
				Vector2 minPos = Vector2::Zero;		//!< 表示する最小座標
				Vector2 maxPos = Vector2::Zero;		//!< 表示する最大座標
			};

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CMiniMap() = default;
			/**
			 * @brief デストラクタ
			*/
			~CMiniMap() = default;

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
			 * @brief 更新処理
			*/
			void Update() override final;

		public:		// メンバ関数
			
		private:	// privateなメンバ関数

			/**
			 * @brief ミニマップのスプライトの初期化
			*/
			void InitMiniMapSprite();

			/**
			 * @brief プレイヤーのアイコンのスプライトの初期化
			*/
			void InitPlayerIconSprite();

			/**
			 * @brief ミニマップの最小座標と最大座標を計算
			*/
			void CalcMinAndMaxPos();

			/**
			 * @brief ミニマップの基点を更新
			*/
			void UpdateMiniMapPivot();

			/**
			 * @brief ミニマップの回転を更新
			*/
			void UpdateMiniMapRotate();

			/**
			 * @brief プレイヤーのアイコンを回転を更新
			*/
			void UpdatePlayerIconRotate();

		private:	// データメンバ
			//!< ミニマップの背景用スプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_miniMapBackSR = nullptr;
			//!< ミニマップにスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_miniMapSR = nullptr;
			//!< プレイヤーのアイコンのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_playerIconSR = nullptr;

			SMiniMapSpriteCB m_miniMapSpriteCB;					//!< ミニマップ用の定数バッファ
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート
		};

	}
}