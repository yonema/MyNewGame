#pragma once
#include "StopWatch.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsUI { class CGameMainUI; }	//!< ゲームメインのUIクラス

	/**
	 * @brief ゲームステート関連のネームスペース
	*/
	namespace nsGameState
	{
		/**
		 * @brief ゲームメインのステートクラス
		*/
		class CGameMainState : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGameMainState()
			{
				m_instance = this;
			}
			/**
			 * @brief デストラクタ
			*/
			~CGameMainState()
			{
				m_instance = nullptr;
			}

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
			 * @brief ゲームのタイムの計測開始
			*/
			void StartTimingGame()
			{
				m_isTimeGame = true;	// タイムを計る
				m_stopWatch.Start();	// ストップウォッチの計測開始
			}

			/**
			 * @brief ゲームのタイムの計測終了
			*/
			void StopTimingGame()
			{
				m_isTimeGame = false;	// タイムを計らない
				m_stopWatch.Stop();		// ストップウォッチの計測終了
			}

			/**
			 * @brief ゲームのタイムを得る
			 * @return 
			*/
			float GetGameTime() const
			{
				return m_gameTimer;
			}

			/**
			 * @brief ゴールした
			*/
			void Goal();

			/**
			 * @brief ゴールしたか？を得る
			 * @return ゴールしたか？
			*/
			bool IsGoal()const
			{
				return m_isGoal;
			}

		public:		// staticなメンバ関数とデータメンバ

			static CGameMainState* m_instance;	//!< インスタンス

			/**
			 * @brief インスタンスを得る
			 * @return インスタンス
			*/
			static CGameMainState* GetInstance()
			{
				return m_instance;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief ゲームのタイムを計測する
			*/
			void TimeGame();

		private:	// データメンバ

			float m_gameTimer = 0.0f;	//!< ゲームのタイマー
			bool m_isTimeGame = false;	//!< ゲームのタイムを計るか？
			nsTimer::CStopWatch m_stopWatch;	//!< ストップウォッチ

			nsUI::CGameMainUI* m_gameMainUI = nullptr;	//!< ゲームメインのUIクラス

			bool m_isGoal = false;		//!< ゴールしているか？

		};

		/**
		 * @brief ゲームメインステートのインスタンスを得る
		 * @return ゲームメインステートのインスタンス
		*/
		static CGameMainState* GameMainState()
		{
			return CGameMainState::GetInstance();
		}

	}
}