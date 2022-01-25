#include "stdafx.h"
#include "PlayerModelAnimation.h"
#include "Player.h"
#include "ModelRender.h"

namespace nsNinjaAttract
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 共通データを使用可能にする
		using namespace nsCommonData;
		// プレイヤーの定数データを使用可能にする
		using namespace nsPlayerConstData::nsModelAnimationConstData;

		/**
		* @brief デストラクタ
		*/
		CPlayerModelAnimation::~CPlayerModelAnimation()
		{
			// モデル解放
			ReleaseModel();

			return;
		}

		/**
		 * @brief モデル初期化
		 * @param[in] player プレイヤーの参照
		*/
		void CPlayerModelAnimation::Init(const CPlayer& player)
		{
			// プレイヤーの参照をセット
			m_playerRef = &player;

			// アニメーション初期化
			InitAnimation();
			// モデル初期化
			InitModel();

			// 手のボーンIDを取得
			m_handBoneID = m_playerModel->FindBoneID(kHandBoneName);

			return;
		}

		/**
		 * @brief プレイヤーモデルクラスのアップデートを実行する
		*/
		void CPlayerModelAnimation::ExecuteUpdate()
		{
			// トランスフォームを更新
			UpdateTransform();
			// アニメーション遷移を更新
			UpdateAnimationTransition();

			return;
		}



		/**
		 * @brief アニメーション初期化
		*/
		void CPlayerModelAnimation::InitAnimation()
		{
			// アニメーションの初期化

			for (int i = 0; i < enAnim_num; i++)
			{
				// アニメーションのロード
				m_animationClips[i].Load(kAnimationFilePath[i]);
				//m_animationClips[i].SetLoopFlag(true);
				// ループフラグを設定
				switch (i)
				{
				// ループするアニメーション
				case enAnim_idle:
				case enAnim_ninjaIdle:
				case enAnim_walk:
				case enAnim_run:
				case enAnim_airIdle:
				case enAnim_swinging:
					m_animationClips[i].SetLoopFlag(true);
					break;
				// ループしないアニメーション
				default:
					m_animationClips[i].SetLoopFlag(false);
					break;
				}
			}

			return;
		}

		/**
		 * @brief モデル初期化
		*/
		void CPlayerModelAnimation::InitModel()
		{

			// プレイヤーモデルレンダラーの生成
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// 座標と回転を設定
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());

			// プレイヤーモデルレンダラーの初期化
			//m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num);
			m_playerModel->IniTranslucent(
				kPlayerModelFilePath,
				m_animationClips,
				enAnim_num
				);
			// シャドウキャスターを有効にする
			m_playerModel->SetIsPlayerShadowCaster(true);

			return;
		}

		/**
		* @brief モデル解放
		*/
		void CPlayerModelAnimation::ReleaseModel()
		{
			// 初期化していないか？
			if (!m_playerModel)
			{
				// 初期化していない
				// 早期リターン
				return;
			}

			// プレイヤーモデルレンダラーの破棄
			DeleteGO(m_playerModel);

			return;
		}

		/**
		 * @brief トランスフォームを更新
		*/
		void CPlayerModelAnimation::UpdateTransform()
		{
			// 座標と回転を更新
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());
			return;
		}


		/**
		 * @brief アニメーション遷移を更新
		*/
		void CPlayerModelAnimation::UpdateAnimationTransition()
		{
			// アニメーションステートを更新
			UpdateAnimationState();

			// アニメーションのスピードを設定
			m_playerModel->SetAnimationSpeed(m_aninSpeed);

			// アニメーション再生
			// 前フレームと同じアニメーションをPlayした場合、なにもしない
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}

		/**
		 * @brief アニメーションステートを更新
		*/
		void CPlayerModelAnimation::UpdateAnimationState()
		{
			// デフォルトのアニメーション補完時間にする
			m_AnimInterpolateTime = kDefaultAnimInterpolateTime;
			// デフォルトのアニメーションスピードにする
			m_aninSpeed = kDefaultAnimSpeed;

			// スイング中のアニメーションステートを更新
			if (UpdateSwingAnimationState())
			{
				return;
			}

			// コマンド入力が失敗した時のアニメーションステートを更新
			if (UpdateCommandFailed())
			{
				return;
			}

			// 敵の上に乗ってるときにアニメーションステートを更新
			if (UpdateOnEnemyAnimationState())
			{
				return;
			}

			// 最初の落下のアニメーションの更新
			if (UpdateStartFall())
			{
				return;
			}

			// ジャンプと空中でのアニメーションの更新
			if (UpdateJumpAndAir())
			{
				return;
			}

			// アイドル状態と歩きと走りのアニメーションの更新
			if (UpdateIdleWalkAndRun())
			{
				return;
			}


			return;
		}

		/**
		 * @brief スイング中のアニメーションステートを更新
		 * @param 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateSwingAnimationState()
		{
			if (m_playerRef->GetState() != nsPlayerConstData::enSwing)
			{
				// スイング中ではないので、早期リターンしないようにする。
				return false;
			}

			// スイング中ののステートに応じて、処理を振り分ける
			switch (m_swingAnimSate)
			{
			case enSwingAnim_swingStart:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingStart)
				{
					// スイングスタートのアニメーションが終了したら

					// スイング中のアニメーションステートをスイング中に遷移
					m_swingAnimSate = enSwingAnim_swing;
				}

				// スイングスタートのアニメーションを再生
				m_animState = enAnim_swingStart;

				break;
			case enSwingAnim_swing:
				// スイング中のアニメーションを再生
				m_animState = enAnim_swinging;

				break;
			case enSwingAnim_swingRaiseLeg:
				// 足を上げるスイングのアニメーションを再生
				m_animState = enAnim_swingRaiseLeg;
				m_aninSpeed = 1.5f;
				break;
			case enSwingAnim_swingRoll:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingRoll)
				{
					// スイングロールのアニメーションが終了したら

					// スイング中のステートをスイング後の空中状態へ遷移
					m_swingAnimSate = enSwingAnim_airAfterSwing;
				}
				// スイングロールのアニメーションを再生
				m_animState = enAnim_swingRoll;
				m_AnimInterpolateTime = 0.0f;
				m_aninSpeed = 2.0f;
				break;
			case enSwingAnim_airAfterSwing:
				// 空中のアイドル状態のアニメーションを再生
				m_animState = enAnim_airIdle;
				break;
			}

			// スイング中のため、早期リターンをするようにする。
			return true;
		}


		/**
		 * @brief コマンド入力が失敗した時のアニメーションステートを更新
		 * @return 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateCommandFailed()
		{
			if (m_playerRef->GetCatchEnemy().GetCatchEnemyState() ==
				nsPlayerConstData::nsCatchEnemyConstData::enCE_FailureCommand)
			{
				// コマンド入力失敗時

				// ノックダウンのアニメーションの開始
				m_animState = enAnim_knockDown;
				m_AnimInterpolateTime = 0.5f;
			}

			if (m_animState != enAnim_knockDown && m_animState != enAnim_standUp)
			{
				// ノックダウン中でも、起き上がり中でもないとき
				// 早期リターンを行わないようにする
				return false;
			}

			// ノックダウン中、または、起き上がり中の時

			if (m_playerModel->IsAnimationPlaying() == true)
			{
				// アニメーションが再生中なら、そのまま再生しておく。（ノックダウン中）
				// 早期リターンを行うようにする。
				return true;
			}
			else
			{
				// アニメーションの再生が終了した時
				if (m_animState == enAnim_knockDown)
				{
					// ノックダウンのアニメーションが終了した時

					// 起き上がり中のアニメーションを再生
					m_animState = enAnim_standUp;
					m_AnimInterpolateTime = 1.0f;
					return true;
				}
				else
				{
					// その他のアニメーション（起き上がり中）が終了した時

					// アイドル状態のアニメーションを再生
					m_animState = enAnim_idle;
					m_AnimInterpolateTime = 0.5f;
				}

			}

			// 早期リターンを行うようにする
			return true;
		}


		/**
		 * @brief 敵の上に乗ってるときのアニメーションステートを更新
		 * @return 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateOnEnemyAnimationState()
		{
			if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
				nsPlayerConstData::nsOnEnemyConstData::enOnEnemy)
			{
				// 敵の上に乗っている時

				// 忍者アイドル状態のアニメーションを再生
				m_animState = enAnim_ninjaIdle;

				// 早期リターンを行うようにする
				return true;
			}
			else if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
				nsPlayerConstData::nsOnEnemyConstData::enGoOnEnemy)
			{
				// 敵の上に向かっている時

				if (m_animState == enAnim_goOnEnemy && m_playerModel->IsAnimationPlaying() != true)
				{
					// アニメーションが、敵の上に向かっているアニメーションとき、かつ
					// アニメーションの再生が終了しているとき

					// 敵の上に向かうアニメーション2を再生
					m_animState = enAnim_goOnEnemy2;
					m_aninSpeed = 2.0f;
				}
				else if (m_animState != enAnim_goOnEnemy2)
				{
					// アニメーションが、敵の上に向かうアニメーション2ではないとき

					// 敵の上に向かうアニメーションを再生
					m_animState = enAnim_goOnEnemy;
					m_aninSpeed = 2.0f;
					m_AnimInterpolateTime = 0.5f;
				}

				// 早期リターンを行うようにする
				return true;
			}

			// 早期リターンを行わないようにする
			return false;
		}

		/**
		 * @brief 最初の落下のアニメーション
		 * @return 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateStartFall()
		{
			if (m_playerRef->GetState() != nsPlayerConstData::enStartFall ||
				m_playerRef->GetPlayerMovement().IsAir() == true)
			{
				// 最初の落下の時ではない、または、空中にいるとき
				// 早期リターンを行わないようにする
				return false;
			}


			// 最初の落下のとき、かつ、空中にいない時（着地時）

			if (m_animState == enAnim_landing && m_playerModel->IsAnimationPlaying() != true)
			{
				// アニメーションが着地のアニメーション、かつ、アニメーションの再生が終了しているとき

				// アイドル状態のアニメーションを再生
				m_animState = enAnim_idle;
				m_AnimInterpolateTime = 0.8f;
			}
			else if (m_animState != enAnim_idle)
			{
				// アニメーションがアイドル状態ではない時

				// 着地のアニメーションを再生
				m_animState = enAnim_landing;
				m_AnimInterpolateTime = 0.2f;
			}

			// 早期リターンを行うようにする
			return true;

		}


		/**
		 * @brief ジャンプと空中でのアニメーション
		 * @return 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateJumpAndAir()
		{
			if (m_playerRef->GetPlayerMovement().IsAir() != true &&
				m_playerRef->GetPlayerMovement().GetPlayerWalkAudRun().IsJumpUp() != true)
			{
				// 空中にいないとき、かつ、ジャンプの上昇中ではないとき
				// 早期リターンを行わないようにする
				return false;
			}

			// 空中にいるとき、または、ジャンプの上昇中ととき

			if (m_animState != enAnim_airIdle)
			{
				// アニメーションが空中アイドル状態ではない

				if (m_playerModel->IsAnimationPlaying() == true)
				{
					// アニメーションが再生中ととき

					// ジャンプの上昇中のアニメーションを再生
					m_animState = enAnim_jumpUp;
					m_AnimInterpolateTime = 0.2f;
					m_aninSpeed = 2.0f;

				}
				else
				{
					// アニメーションの再生が終了したとき

					// 空中アイドル状態のアニメーションを再生
					m_animState = enAnim_airIdle;
				}
			}

			// 早期リターンを行うようにする
			return true;
		}

		/**
		 * @brief アイドル状態と歩きと走りのアニメーションの更新
		 * @return 早期リターンを行うか？
		*/
		bool CPlayerModelAnimation::UpdateIdleWalkAndRun()
		{
			// プレイヤーの速度
			const float playerVelocity = m_playerRef->GetPlayerMovement().GetXZSpeed();

			if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kMinSpeed + 10.0f)
			{
				// 速度が最低速度以下

				// アイドル状態のアニメーションを再生
				m_animState = enAnim_idle;

				// 早期リターンを行うようにする
				return true;
			}
			else if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed + 50.0f)
			{
				// 速度が歩きの最大速度以下

				// 歩きのアニメーションを再生
				m_animState = enAnim_walk;

				// 早期リターンを行うようにする
				return true;
			}
			else
			{
				// 速度が歩く最大速度より速い

				// 走りのアニメーションを再生
				m_animState = enAnim_run;
				// 速度に応じて、1.0f～1.5fの速さでアニメーションを再生
				m_aninSpeed = 1.0f + 0.5f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kRunMaxSpeed;

				// 早期リターンを行うようにする
				return true;
			}

			// 早期リターンを行わないようにする
			return false;
		}

	}
}