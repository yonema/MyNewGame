#pragma once
#include "Noncopyable.h"
#include "PlayerInput.h"
#include "SpringCamera.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		struct SPlayerInputData;	// プレイヤーの入力情報構造体

		/**
		 * @brief プレイヤーのカメラクラス
		*/
		class CPlayerCamera : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerCamera() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPlayerCamera() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] startTargetPos スタート注視点
			 * @param[in] inputData 入力情報
			*/
			void Init(const Vector3& startTargetPos, const SPlayerInputData& inputData);

			/**
			 * @brief プレイヤーカメラクラスのアップデートを実行する
			*/
			void ExecuteUpdate();

			/**
			 * @brief カメラの前方向のベクトルを取得
			 * @return カメラの前方向のベクトル
			*/
			const Vector3& GetCameraForward() const
			{
				return m_camera->GetForward();
			}

			/**
			 * @brief カメラの右方向のベクトルを取得
			 * @return カメラの右方向のベクトル
			*/
			const Vector3& GetCameraRight() const
			{
				return m_camera->GetRight();
			}

		private:	// privateなメンバ関数

			/**
			 * @brief カメラの回転を計算する
			*/
			void CalcCameraRotation();

			/**
			 * @brief カメラの視点と注視点を計算する
			 * @param[out] pos_out 視点が格納される
			 * @param[out] targetPos_out 注視点が格納される
			*/
			void CalcCameraPositionAndTargetPos(
				Vector3* pos_out,
				Vector3* targetPos_out
			);

			/**
			 * @brief バネカメラを更新する
			 * @param[in] pos 視点
			 * @param[in] targetPos 注視点
			*/
			void UpdateSpringCamera(const Vector3& pos, const Vector3& targetPos);

		private:	// データメンバ
			Camera* m_camera = nullptr;				//!< カメラ
			nsCamera::CSpringCamera m_springCamera;	//!< バネカメラ
			Vector3 m_toCameraVec = Vector3::Zero;	//!< 注視点から視点へのベクトル

			const Vector3* m_playerPosition = nullptr;				//!< プレイヤーの座標の参照
			const SPlayerInputData* m_playerInputData = nullptr;	//!< プレイヤーの入力情報の参照

		};

	}
}