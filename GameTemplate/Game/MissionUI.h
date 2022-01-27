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
			 * @brief ミッションを表示する処理を更新
			*/
			void UpdataShowMission();

			/**
			 * @brief ミッションを非表示にする処理を更新
			*/
			void UpdateHideMission();

			/**
			 * @brief ミッションを一つクリアした時の処理の更新
			*/
			void UpdateClearOneMission();

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
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_checkMarkSRs;	//!< チェックマークのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionWindowSR = nullptr;	//!< ミッションウィンドウのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionResultFrameSR = nullptr;	//!< ミッションリザルトの枠のスプライトレンダラー
			//!< ミッションリザルトのテキストのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_missionResultTextSRs;

			nsGraphic::nsFont::CFontRender* m_clearTimeFR = nullptr;	//!< クリアタイムのフォントレンダラー
			nsGraphic::nsFont::CFontRender* m_numOfMissFR = nullptr;	//!< ミスの回数のフォントレンダラー

			std::vector<Vector3> m_checkMarkOffsets;	//!< チェックマークのオフセット
			std::vector<bool> m_checkMarkFlag;			//!< チェックマークのクリアフラグ
			float m_timer = 0.0f;	//!< タイマー

			//!< ミッションステート
			nsMissionUIConstData::EnMissionState m_missionState = nsMissionUIConstData::enMS_none;
			//!< 現在のクリアしたミッションのタイプ
			nsMissionUIConstData::EnMissionType m_currentClearMissionType = 
				static_cast<nsMissionUIConstData::EnMissionType>(-1);
		};

	}
}