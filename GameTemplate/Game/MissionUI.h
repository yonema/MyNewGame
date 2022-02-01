#pragma once
#include "UIConstData.h"

// 前方宣言
class Level2D;

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラー
		namespace nsFont { class CFontRender; }		// フォントレンダラー
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsSound { class CSoundCue; }


	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		/**
		 * @brief ミッションを表示するUIクラス
		*/
		class CMissionUI : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CMissionUI() = default;
			/**
			 * @brief デストラクタ
			*/
			~CMissionUI() = default;

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
			 * @brief ミッションを表示する
			*/
			void ShowMission();

			/**
			 * @brief ミッションを一つクリアした時の処理
			 * @param[in] missionType クリアしたミッションのタイプ
			*/
			void ClearOneMission(const nsMissionUIConstData::EnMissionType missionType);

		private:	// privateなメンバ関数

			/**
			 * @brief スプライトの初期化
			*/
			void InitSprite();

			/**
			 * @brief フォントの初期化
			*/
			void InitFont();

			/**
			 * @brief サウンドの初期化
			*/
			void InitSound();

			/**
			 * @brief クリアフラグをチェックする処理を更新
			*/
			void UpdateCheckClearFlag();

			/**
			 * @brief ミッションを表示する処理を更新
			 * @param[in] startPos 表示アニメーションの開始座標
			 * @param[in] endPos 表示アニメーションの終了座標
			*/
			void UpdateShowMission(const Vector3& startPos, const Vector3& endPos);

			/**
			 * @brief ミッションを非表示にする処理を更新
			*/
			void UpdateHideMission();

			/**
			 * @brief ミッションを一つクリアした時の処理の更新
			*/
			void UpdateClearOneMission();

			/**
			 * @brief 全てのミッションをクリアした時の処理の更新
			*/
			void UpdateClearAllMission();

			/**
			 * @brief リザルトの時の処理の更新
			*/
			void UpdateResult();

			/**
			 * @brief サブミッションをクリアしたかどうかを調べる
			*/
			void CheckClearSubMission();

			/**
			 * @brief 全てのミッションをクリアした時のフレームの点滅処理
			*/
			void BlinkClearAllMissionFrame();

			//void BlinkSprite(nsGraphic::nsSprite::CSpriteRender* sprite, float* timer, const float time)

			/**
			 * @brief ミッションのスプライトレンダラー全てにクエリを行う
			 * @param[in] func 実行する関数
			*/
			void QueryAllMissionSRs(const std::function<void(nsGraphic::nsSprite::CSpriteRender* spriteRender)> func);

			/**
			 * @brief ステートを遷移する
			 * @param[in] newState 新しいステート
			*/
			void ChangeState(nsMissionUIConstData::EnMissionState newState);

		private:	// データメンバ
			Level2D* m_missionLevel = nullptr;	//!< ミッション用のレベル2Dクラス
			//!< チェックマークのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_checkMarkSRs[nsMissionUIConstData::enMissionTypeNum];
			//!< ミッションウィンドウのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionWindowSR = nullptr;
			//!< ミッションリザルトの枠のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionResultFrameSR = nullptr;
			//!< ミッションリザルトのテキストのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionResultTextSRs[nsMissionUIConstData::enMissionResultTypeNum] = {};
			//!< ミッションをすべてクリアした時のテキストのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionAllClearTextSR = nullptr;
			//!< ミッションをすべてクリアした時のフレームのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionAllClearFrameSR = nullptr;
			//!< 終わりへと行くスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_toEndSR = nullptr;
			float m_toEndBlinkTimer = 0.0f;		//!< 終わりへと行くスプライトの点滅用タイマー

			//!< ミッションをすべてクリアした時のスプライトのオフセット
			Vector3 m_missionAllClearTextSpriteOffset = Vector3::Zero;
			Vector3 m_missionAllClearFrameSpriteOffset = Vector3::Zero;
			float m_blinkTimer = 0.0f;	//!< 点滅用タイマー

			nsGraphic::nsFont::CFontRender* m_clearTimeFR = nullptr;	//!< クリアタイムのフォントレンダラー
			nsGraphic::nsFont::CFontRender* m_numOfMissFR = nullptr;	//!< ミスの回数のフォントレンダラー
			float m_clearTimeTimer = 0.0f;								//!< クリアタイムのタイマー
			float m_numOfMissTimer = 0.0f;								//!< ミスの回数のタイマー

			Vector3 m_checkMarkOffsets[nsMissionUIConstData::enMissionTypeNum];	//!< チェックマークのオフセット
			bool m_checkMarkFlag[nsMissionUIConstData::enMissionTypeNum];		//!< チェックマークのクリアフラグ
			float m_timer = 0.0f;	//!< タイマー

			//!< ミッションステート
			nsMissionUIConstData::EnMissionState m_missionState = nsMissionUIConstData::enMS_none;
			//!< 現在のクリアしたミッションのタイプ
			nsMissionUIConstData::EnMissionType m_currentClearMissionType = 
				static_cast<nsMissionUIConstData::EnMissionType>(-1);
			//!< ミッションのリザルトのステート
			nsMissionUIConstData::EnMissionResultState m_missionResultState =
				nsMissionUIConstData::enMRS_showMission;

			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート

			nsSound::CSoundCue* m_opneMission = nullptr;		//!< ミッションを表示するときのサウンド
			nsSound::CSoundCue* m_closeMission = nullptr;		//!< ミッションを非表示にするときにサウンド
			nsSound::CSoundCue* m_clearOneMission = nullptr;	//!< ミッションを一つクリアした時にサウンド
			nsSound::CSoundCue* m_clearAllMission = nullptr;	//!< ミッションをすべてクリアした時のサウンド
			nsSound::CSoundCue* m_showResult = nullptr;			//!< リザルトを表示する時のサウンド
			nsSound::CSoundCue* m_resultPerfect = nullptr;		//!< リザルトがパーフェクトだったときのサウンド

		};

	}
}