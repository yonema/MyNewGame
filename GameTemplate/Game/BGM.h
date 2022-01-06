#pragma once

namespace nsMyGame
{
	namespace nsPlayer { class CPlayer; }
	namespace nsSound { class CSoundCue; }

	namespace nsBGM
	{
		class CBGM : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CBGM() = default;
			/**
			 * @brief デストラクタ
			*/
			~CBGM() = default;

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

			/**
			 * @brief 初期化
			 * @param[in] player プレイヤーの参照
			*/
			void Init(const nsPlayer::CPlayer& player);

		private:	// データメンバ
			const nsPlayer::CPlayer* m_playerRef = nullptr;		//!< プレイヤークラスの参照

			nsSound::CSoundCue* m_swingBGMSC = nullptr;		//!< スイング中のBGMのサウンド

		};

	}
}