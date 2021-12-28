#pragma once
#include "UIConstData.h"

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
		 * @brief 敵を捕まえる処理関連のUI
		*/
		class CEnemyCatchUI : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CEnemyCatchUI() = default;
			/**
			 * @brief デストラクタ
			*/
			~CEnemyCatchUI() = default;

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
			 * @brief 敵を捕まえることができる合図のスプライトの初期化
			*/
			void InitCanCatchEnemeySprite();

			/**
			 * @brief 敵を捕まえることができる合図のスプライトの更新
			*/
			void UpdateCanCatchEnemySprite();


		private:	// データメンバ
			//!< 敵を捕まえることができる合図のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* 
				m_canCatchEnemySR[nsEnemyCatchUIConstData::enCanCatchEnemySpriteNum] = {};
			//!< 敵を捕まえることができる合図のスプライトに使うタイマー
			float m_canCatchEnemyTimer = 0.0f;

			std::vector<const nsAICharacter::CAICar*> m_aiCarsRef;	//!< 車たちのconst参照

			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート

		};

	}
}