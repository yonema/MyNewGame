#pragma once

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラー
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }

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
			 * @brief 車のアイコンのスプライトの初期化
			*/
			void InitCarIconSprite();

			/**
			 * @brief 車のミニアイコンのスプライトの初期化
			*/
			void InitCarMiniIconSprite();

			/**
			 * @brief ミニマップの最小座標と最大座標を計算
			*/
			void CalcMinAndMaxPos();

			/**
			 * @brief クラス内のいろんなところで共通して使用されるデータの更新
			*/
			void UpdateCommonData();

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

			/**
			 * @brief 車のアイコンを更新
			*/
			void UpdateCarIcon();

			/**
			 * @brief 車のミニアイコンを更新
			*/
			void UpdateCarMiniIcon();

			/**
			 * @brief 車のミニアイコンの画面はみだし用を更新。
			 * この関数は、UpdateCarMiniIconで呼ばれる。
			 * @param[in] index 車のインデックス
			 * @param[in] isIntersect 交差しているか？
			 * @param[in] playerToCarVec プレイヤーから車へのベクトル
			 * @param[in] iconPos 車のアイコンの座標
			*/
			void UpdateCarMiniIconOut(
				const int index,
				const bool isIntersect,
				const Vector3& playerToCarVec,
				const Vector2& iconPos
			);

		private:	// データメンバ
			//!< ミニマップの背景用スプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_miniMapBackSR = nullptr;
			//!< ミニマップの枠用スプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_miniMapFrameSR = nullptr;
			//!< ミニマップのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_miniMapSR = nullptr;
			//!< プレイヤーのアイコンのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_playerIconSR = nullptr;
			//!< 車のアイコンのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carIconSRs;
			//!< 車のミニアイコンのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carMiniIconSRs;
			//!< 車のミニアイコンの画面はみだし用スプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_carMiniIconOutSRs;

			//////// 共通パラメータ ////////
			const Vector3* m_playerPositionRef = nullptr;			//!< プレイヤーの座標の参照
			std::vector<const nsAICharacter::CAICar*> m_aiCarsRef;	//!< 車たちのconst参照
			Vector3 m_cameraForwardXZ = Vector3::Front;				//!< XZ平面でのカメラの前方向
			Vector3 m_cameraRightXZ = Vector3::Right;				//!< XZ平面でのカメラの前方向
			Quaternion m_miniMapRotatioin = Quaternion::Identity;	//!< ミニマップの回転

			SMiniMapSpriteCB m_miniMapSpriteCB;					//!< ミニマップ用の定数バッファ
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート
		};

	}
}