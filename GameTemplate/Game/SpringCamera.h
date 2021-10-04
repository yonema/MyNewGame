#pragma once
#include "Noncopyable.h"
#include "CameraCollisionSolver.h"

namespace nsMyGame
{
	/**
	 * @brief カメラ関連のネームスペース
	*/
	namespace nsCamera
	{
		/**
		 * @brief ばねカメラクラス
		*/
		class CSpringCamera : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSpringCamera() = default;
			/**
			 * @brief デストラクタ
			*/
			~CSpringCamera() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param camera カメラ
			 * @param maxMoveSpeed カメラの最大速度
			 * @param isEnableCollisionSolver コリジョン処理を有効にするか？
			 * @param sphereCollisionRadius 球体コリジョンの半径。isEnableCollisionSolverがtrueのときのみ有効。
			*/
			void Init(
				Camera& camera,
				const float maxMoveSpeed,
				const bool isEnableCollisionSolver = false,
				const float sphereCollisionRadius = 0.0f
			);

			/**
			 * @brief 注視点を設定
			 * @param[in] target 注視点
			*/
			void SetTarget(const Vector3& target)
			{
				m_target = target;
			}

			/**
			 * @brief 視点を設定
			 * @param[in] position 視点
			*/
			void SetPosition(const Vector3& position)
			{
				m_position = position;
			}

			/**
			 * @brief 遠平面を設定
			 * @param[in] _far 遠平面
			*/
			void SetFar(const float _far)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetFar(_far);
			}

			/**
			 * @brief 近平面を設定
			 * @param[in] _near 近平面
			*/
			void SetNear(const float _near)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetNear(_near);
			}

			/**
			 * @brief カメラの回転行列を取得
			 * @return カメラの回転行列
			*/
			const Matrix& GetCameraRotation() const
			{
				if (m_camera == nullptr) {
					return Matrix::Identity;
				}
				return m_camera->GetCameraRotation();
			}

			/**
			 * @brief 注視点を取得
			 * @attention この関数が戻してくる注視点は、実際の注視点ではないので注意してください。
			 * 実際の注視点がほしい場合はGetRealTargetを使用してください。
			 * @return 注視点
			*/
			const Vector3& GetTarget() const
			{
				return m_target;
			}

			/**
			 * @brief 視点を取得
			 * @attention この関数が返してくる視点は、実際の視点ではないので注意してください。
			 * 実際の始点がほしい場合はGetRealPositionを使用して下さい。
			 * @return 視点
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief 実際の注視点を取得。
			 * @return 注視点
			*/
			const Vector3& GetRealTarget() const
			{
				if (m_camera == nullptr) {
					return Vector3::Zero;
				}
				return m_camera->GetTarget();

			}

			/**
			 * @brief 視点を取得
			 * @return 視点
			*/
			const Vector3& GetRealPosition() const
			{
				if (m_camera == nullptr) {
					return Vector3::Zero;
				}
				return m_camera->GetPosition();
			}

			/**
			 * @brief 更新
			*/
			void Update();

			/**
			 * @brief バネの減衰率を設定。値が大きいほどカメラが遅れて付いてきます。
			 * @param[in] dampingRate バネの減衰率
			*/
			void SetDampingRate(const float dampingRate)
			{
				m_targetDampingRate = dampingRate;
			}

			/**
			 * @brief 画角を設定
			 * @param[in] angle 画角
			*/
			void SetViewAngle(const float angle)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetViewAngle(angle);
			}

			/**
			 * @brief 画角を取得
			 * @return 画角
			*/
			float GetViewAngle() const
			{
				if (m_camera == nullptr) {
					return 0.0f;
				}
				return m_camera->GetViewAngle();
			}

			/**
			 * @brief リフレッシュ
			 * @details この関数を呼び出すと、バネの移動がリフレッシュされて、Update関数を呼び出すと
			 * カメラの位置が即座に目標視点と目標注視点に移動します。
			 * シーンが切り替わってカメラを一気に切り替えたいときなどに使用してください。
			*/
			void Refresh()
			{
				m_targetMoveSpeed = Vector3::Zero;
				m_positionMoveSpeed = Vector3::Zero;
				m_isRefresh = true;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief ばねカメラの更新処理
			*/
			void UpdateSpringCamera();

			/**
			 * @brief カメラの更新処理
			*/
			void UpdateCamera()
			{
				if (m_camera) {
					m_camera->Update();
				}
			}
		private:
			Camera* m_camera = nullptr;						//!< カメラ。
			Vector3 m_target = Vector3::Zero;				//!< 目標となる注視点。
			Vector3	m_position = Vector3::Zero;				//!< 目標となる視点。
			Vector3	m_targetMoveSpeed = Vector3::Zero;		//!< 注視点の移動速度。
			Vector3	m_positionMoveSpeed = Vector3::Zero;	//!< 視点の移動速度。
			float m_maxMoveSpeed = 0.0f;					//!< 最高移動速度。
			float m_targetDampingRate = 1.0f;				//!< 減衰率。値が大きいほどカメラが遅れ付いてくる。
			float m_dampingRate = 1.0f;						//!< 減衰率。
			float m_dampingRateVel = 0.0f;
			bool m_isEnableCollisionSolver = false;			//!< コリジョン処理が有効？
			bool m_isRefresh = true;						//!< リフレッシュが必要？
			CCameraCollisionSolver m_cameraCollisionSolver;	//!< カメラコリジョン解決クラス
		};
	}
}


