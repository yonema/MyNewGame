#pragma once
#include "BGMConstData.h"

namespace nsNinjaAttract
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

		private:	// privateなメンバ関数

			/**
			 * @brief クロスフェードを設定する
			 * @param[in] bgmType 次のBGMの種類
			*/
			void SetCrossFade(const nsBGMConstData::EnBGMType bgmType);

			/**
			 * @brief BGMのステートを更新する
			*/
			void UpdateBGMState();

			/**
			 * @brief クロスフェードを更新
			*/
			void UpdateCrossFade();

			/**
			 * @brief ステート遷移
			 * @param[in] newState 新しいステート
			*/
			void ChangeState(const nsBGMConstData::EnBGMType newState);

		private:	// データメンバ
			const nsPlayer::CPlayer* m_playerRef = nullptr;		//!< プレイヤークラスの参照

			//!< スイング中のBGMのサウンド
			nsSound::CSoundCue* m_bgmSC[nsBGMConstData::enBGMTypeNum] = {};

			//!< 現在のBGMの種類
			nsBGMConstData::EnBGMType m_currentBGMType = nsBGMConstData::enBT_Normal;
			//!< 次のBGMの種類
			nsBGMConstData::EnBGMType m_nextBGMType = nsBGMConstData::enBT_Normal;
			//!< BGMのステート
			nsBGMConstData::EnBGMType m_bgmState = nsBGMConstData::enBT_Normal;
			bool m_isCrossFade = false;					//!< クロスフェード中か？

			float m_crossFadeTimer = 0.0f;				//!< クロスフェードに使用するタイマー
			float m_crossFadeTime = 0.0f;				//!< クロスフェードのタイム
		};

	}
}