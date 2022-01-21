#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"
#include "SavedPlayerInputDataConstData.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer{ class CPlayer; }						// プレイヤークラス
	namespace nsExternalData { class CSavedPlayerInputData; }	// プレイヤーの入力情報の書き出しクラス


	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーの入力情報構造体
		*/
		struct SPlayerInputData
		{
			float axisMoveForward = 0.0f;			//!< 前、後移動の軸入力情報
			float axisMoveRight = 0.0f;				//!< 右、左移動の軸入力情報
			bool inputMoveAxis = false;				//!< 移動の軸入力があったか？
			float axisCameraRotVertical = 0.0f;		//!< カメラの縦回転の軸入力情報
			float axisCameraRotHorizontal = 0.0f;	//!< カメラの横回転の軸入力情報
			bool inputCameraAxis = false;			//!< カメラの軸入力があったか？
			bool actionJump = false;				//!< ジャンプの入力情報
			bool actionDush = false;				//!< ダッシュの入力情報
			bool actionSwing = false;				//!< スイングの入力情報
			bool actionSearchEnemy = false;			//!< 敵を探知する入力情報
			bool actionCatchEnemy = false;			//!< 敵を捕まえる入力情報
			//!< コマンド入力情報
			nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType inputCommand =
				nsPlayerConstData::nsCatchEnemyConstData::enQTE_None;
		};

		struct SPlayerInputDataAndDeltaTime
		{
			SPlayerInputData playerInputData;	//!< プレイヤーの入力データ
			float deltaTime = 0.0f;				//!< デルタタイム
		};

		/**
		 * @brief プレイヤー入力クラス
		*/
		class CPlayerInput : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerInput();
			/**
			 * @brief デストラクタ
			*/
			~CPlayerInput();

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in,out] player プレイヤーの参照
			*/
			void Init(CPlayer* player);

			/**
			 * @brief 入力処理を実行する
			*/
			void ExecuteUpdate();

			/**
			 * @brief プレイヤーの入力情報を得る
			 * @return プレイy－の入力情報
			*/
			const SPlayerInputData& GetPlayerInputData() const
			{
				return m_playerInputDataAndDeltaTime.playerInputData;
			}

			/**
			 * @brief プレイヤー全体で使用するデルタタイムを取得
			 * @return プレイヤー全体で使用するデルタタイム
			*/
			float GetDeltaTime() const
			{
				return m_playerInputDataAndDeltaTime.deltaTime;
			}

			/**
			 * @brief 保存する入力データの収集を開始する
			*/
			void StartSaveData()
			{
				m_isStartSaveData = true;
			}

			/**
			 * @brief 保存する入力データの収取を終了して、保存する。
			 * @param[in] saveFileType 保存ファイルの種類
			*/
			void EndSaveDataAndSave(
				const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
			);

			/**
			 * @brief 保存したデータの使用を開始する
			 * @param[in] saveFileType 保存ファイルの種類
			*/
			void StartUsingSavedData(
				const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
			);

			/**
			 * @brief 保存したデータの使用を終了する
			*/
			void EndUsingSavedData();

			/**
			 * @brief ロードしたデータの進捗が、最後まで行っているか？を得る。
			 * @return ロードしたデータの進捗が、最後まで行っているか？
			*/
			bool IsEndLoadDataProgress() const;

		private:	// privateなメンバ関数

			/**
			 * @brief 入力情報を更新する
			*/
			void UpdateInput();

			/**
			 * @brief 軸入力情報を更新する
			*/
			void UpdateInputAxis();

			/**
			 * @brief アクション入力情報を更新する
			*/
			void UpdateInputAction();

			/**
			 * @brief 移動の軸入力情報の更新
			*/
			void OnUpdateInputAxisMove();

			/**
			 * @brief カメラの回転の軸入力情報の更新
			*/
			void OnUpdateInputAxisCameraRot();

			/**
			 * @brief コマンド入力情報の更新
			*/
			void UpdateCommandInputData();

		private:	// データメンバ
			const GamePad* m_pad = nullptr;		//!<  ゲームパッド
			CPlayer* m_playerRef = nullptr;		//!< プレイヤーの参照
			//!< プレイヤーの入力情報とデルタタイム
			SPlayerInputDataAndDeltaTime m_playerInputDataAndDeltaTime;
			bool m_canInputCommandAxis = false;	//!< コマンドの軸入力が可能か？
			nsExternalData::CSavedPlayerInputData* m_savedPlayerInputData = nullptr;
			bool m_isStartSaveData = false;		//!< 保存データの収集を開始しているか？
			bool m_isUsingSavedData = false;	//!< 保存データを使用中か？
		};
	}
}

