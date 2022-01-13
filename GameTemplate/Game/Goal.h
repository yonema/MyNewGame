#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// モデルレンダラー
	namespace nsPlayer { class CPlayer; }	// プレイヤー

	/**
	 * @brief ゴール関連のネームスペース
	*/
	namespace nsGoal
	{
		/**
		 * @brief ゴールクラス
		*/
		class CGoal : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGoal() = default;
			/**
			 * @brief デストラクタ
			*/
			~CGoal() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 更新処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void Update() override final;
		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大率
			 * @param[in] player プレイヤー
			*/
			void Init(
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3 scale,
				nsPlayer::CPlayer& player
				);

		private:

			/**
			 * @brief クルクル回転し続ける
			*/
			void Rotationg();

			/**
			 * @brief ゴールしているかを調べる
			*/
			void CheckIsGoal();

		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_goalMR = nullptr;	//!< ゴールのモデルレンダラー
			const nsPlayer::CPlayer* m_player = nullptr;			//!< プレイヤー
			float m_rotateAngle = 0.0f;
		};

	}
}