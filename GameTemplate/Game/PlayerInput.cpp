#include "stdafx.h"
#include "PlayerInput.h"
#include "Player.h"
#include "SavedPlayerInputData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤー入力クラスの定数データを使用可能にする
		using namespace nsPlayerConstData::nsInputConstData;
		using nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType;

		/**
		 * @brief コンストラクタ
		*/
		CPlayerInput::CPlayerInput()
		{
			m_savedPlayerInputData = new nsExternalData::CSavedPlayerInputData;
			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CPlayerInput::~CPlayerInput()
		{
			delete m_savedPlayerInputData;
			return;
		}

		/**
		 * @brief 初期化
		 * @param[in,out] player プレイヤーの参照
		*/
		void CPlayerInput::Init(CPlayer* player)
		{
			// ゲームパッドの参照を得る
			m_pad = g_pad[0];

			// プレイヤーの参照をセットする
			m_playerRef = player;

			return;
		}

		/**
		 * @brief 入力処理を実行する
		*/
		void CPlayerInput::ExecuteUpdate()
		{
			// 入力情報を初期化する
			memset(&m_playerInputDataAndDeltaTime, 0, sizeof(m_playerInputDataAndDeltaTime));

			// 入力情報を更新する
			UpdateInput();

			if (m_isStartSaveData)
			{
				// 保存データの収集を開始しているなら、保存するデータを追加する。
				m_savedPlayerInputData->PushBuckData(m_playerInputDataAndDeltaTime);
			}

			if (m_playerRef->IsInputtable() != true || m_isUsingSavedData)
			{
				// プレイヤーが入力不可の時、または、保存した入力データを使用中の時
				// 入力情報を初期化する
				memset(
					&m_playerInputDataAndDeltaTime.playerInputData,
					0, 
					sizeof(m_playerInputDataAndDeltaTime.playerInputData));
			}

			if (m_isUsingSavedData)
			{
				// 保存した入力データを使用中の時
				// 現在の進捗のロードしたデータを取り出して、進捗を進める。
				SPlayerInputDataAndDeltaTime* loadData = m_savedPlayerInputData->GetAndProgressLoadData();
				if (loadData)
				{
					// データがあれば、入力データに反映する。
					m_playerInputDataAndDeltaTime = *loadData;
				}
			}

			return;
		}

		/**
		 * @brief 保存する入力データの収取を終了して、保存する。
		 * @param[in] 保存ファイルの種類
		*/
		void CPlayerInput::EndSaveDataAndSave(
			const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
		)
		{
			// 保存データの収集を開始しているか？のフラグを折る
			m_isStartSaveData = false;

			// 収集したデータを保存する
			m_savedPlayerInputData->SaveData(saveFileType);

			// 収集したデータをクリアする
			m_savedPlayerInputData->ClearSaveData();

			return;
		}

		/**
		 * @brief 保存したデータの使用を開始する
		 * @param[in] saveFileType 保存ファイルの種類
		*/
		void CPlayerInput::StartUsingSavedData(
			const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
		)
		{
			// 保存データを使用中フラグをたてる
			m_isUsingSavedData = true;

			// 保存してあるプレイヤーの入力データをロードする
			m_savedPlayerInputData->LoadData(saveFileType);

			return;
		}

		/**
		 * @brief 保存したデータの使用を終了する
		*/
		void CPlayerInput::EndUsingSavedData()
		{
			// 保存データを使用中フラグを折る
			m_isUsingSavedData = false;

			// ロードしてあるデータをクリア
			m_savedPlayerInputData->ClearLoadData();

			return;
		}

		/**
		 * @brief ロードしたデータの進捗が、最後まで行っているか？を得る。
		 * @return ロードしたデータの進捗が、最後まで行っているか？
		*/
		bool CPlayerInput::IsEndLoadDataProgress() const
		{
			return m_savedPlayerInputData->IsEndLoadDataProgress();
		}


		/**
		 * @brief 入力情報を更新する
		*/
		void CPlayerInput::UpdateInput()
		{
			// 軸入力を更新する
			UpdateInputAxis();

			// アクション入力を更新する
			UpdateInputAction();

			// コマンド入力情報の更新
			UpdateCommandInputData();

			// デルタタイムを取得
			m_playerInputDataAndDeltaTime.deltaTime = nsTimer::GameTime().GetFrameDeltaTime();

			return;
		}


		/**
		 * @brief 軸入力情報を更新する
		*/
		void CPlayerInput::UpdateInputAxis()
		{
			// 移動の軸入力情報の更新
			OnUpdateInputAxisMove();

			// カメラの回転の軸入力情報の更新
			OnUpdateInputAxisCameraRot();

			return;
		}


		/**
		 * @brief アクション入力情報を更新する
		*/
		void CPlayerInput::UpdateInputAction()
		{
			// Aボタントリガー
			if (m_pad->IsTrigger(enButtonA))
			{
				// ジャンプ
				m_playerInputDataAndDeltaTime.playerInputData.actionJump = true;
			}

			// 右スティックトリガー
			if (m_pad->IsTrigger(enButtonRB3))
			{
				// 敵を探知
				m_playerInputDataAndDeltaTime.playerInputData.actionSearchEnemy = true;
			}

			// Yボタントリガー
			if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputDataAndDeltaTime.playerInputData.actionCatchEnemy = true;
			}

			// RB2ボタンが押されているか？
			if (m_pad->IsPress(enButtonRB2))
			{
				// ダッシュ
				m_playerInputDataAndDeltaTime.playerInputData.actionDush = true;
				// スイング
				m_playerInputDataAndDeltaTime.playerInputData.actionSwing = true;
			}

			return;
		}

		/**
		 * @brief 移動の軸入力情報の更新
		*/
		void CPlayerInput::OnUpdateInputAxisMove()
		{
			//////// 1.左スティックの入力情報を調べる ////////

			// パッドの左スティックの軸入力情報
			Vector2 padLStickF(m_pad->GetLStickXF(), m_pad->GetLStickYF());
			// 何か正規化してくれてないから、正規化する
			padLStickF.Normalize();

			// Y軸方向の入力を調べる
			m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = padLStickF.y;
			// X軸方向の入力を調べる
			m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = padLStickF.x;

			// 軸入力があった場合は十字キーを調べる必要がないため、早期リターンを行う
			if (fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward) > kInputAxisMin ||
				fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight) > kInputAxisMin)
			{
				// あった
				m_playerInputDataAndDeltaTime.playerInputData.inputMoveAxis = true;
				return;
			}


			//////// 2.十字キーの入力情報を調べる ////////

			// 上、下キーの入力情報を調べる
			if (m_pad->IsPress(enButtonUp))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonDown))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = -kDPadInputPower;
			}
			// 右、左キーの入力情報を調べる
			if (m_pad->IsPress(enButtonRight))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonLeft))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = -kDPadInputPower;
			}

			// 斜め移動の移動の場合は、ルート2倍入力量が多くなるため、ルート2で割る
			if (m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward != 0.0f && 
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight != 0.0f)
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward /= kSquareRootOfTwo;
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight /= kSquareRootOfTwo;
			}

			// 軸入力があったか？
			if (m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward != 0.0f || 
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight != 0.0f)
			{
				// あった
				m_playerInputDataAndDeltaTime.playerInputData.inputMoveAxis = true;
			}

			return;
		}

		/**
		 * @brief カメラの回転の軸入力情報の更新
		*/
		void CPlayerInput::OnUpdateInputAxisCameraRot()
		{
			// 右スティックの軸入力情報を調べる

			// Y軸方向の入力を調べる
			m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotVertical = m_pad->GetRStickYF();
			// X軸方向の入力を調べる
			m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotHorizontal = m_pad->GetRStickXF();

			// 入力があったか？
			if (fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotVertical) > kInputAxisMin ||
				fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotHorizontal) > kInputAxisMin)
			{
				// あった
				m_playerInputDataAndDeltaTime.playerInputData.inputCameraAxis = true;
			}

			return;
		}


		/**
		 * @brief コマンド入力情報の更新
		*/
		void CPlayerInput::UpdateCommandInputData()
		{
			m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_None;

			// ABYXボタンの更新
			if (m_pad->IsTrigger(enButtonA))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_A;
			}
			else if (m_pad->IsTrigger(enButtonB))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_B;
			}
			else if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_Y;
			}
			else if (m_pad->IsTrigger(enButtonX))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_X;
			}
			// 十字キーの更新
			else if (m_pad->IsTrigger(enButtonUp))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
			}
			else if (m_pad->IsTrigger(enButtonDown))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
			}
			else if (m_pad->IsTrigger(enButtonLeft))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
			}
			else if (m_pad->IsTrigger(enButtonRight))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
			}
			// 左スティックの更新
			else if (m_pad->GetLStickYF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickYF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
				m_canInputCommandAxis = false;
			}
			// L1,L2,R1,R2ボタンの更新
			else if (m_pad->IsTrigger(enButtonLB1))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L1;
			}
			else if (m_pad->IsTrigger(enButtonLB2))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L2;
			}
			else if (m_pad->IsTrigger(enButtonRB1))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_R1;
			}
			else if (m_pad->IsTrigger(enButtonRB2))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_R2;
			}
			// 軸入力のフラグの回復
			else if (fabsf(m_pad->GetLStickYF()) <= kInputAxisMin && fabsf(m_pad->GetLStickXF() <= kInputAxisMin))
			{
				m_canInputCommandAxis = true;
			}

			return;
		}
	}
}