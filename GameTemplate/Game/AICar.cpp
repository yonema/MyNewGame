#include "stdafx.h"
#include "AICar.h"

namespace nsMyGame
{
	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief 派生クラス用のUpdateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CAICar::StartSub()
		{
			InitModel(
				"Assets/modelData/car/sedan_car.tkm", 
				{ -1000.0f,10.0f,0.0f },
				Quaternion::Identity, Vector3::One
			);

			return true;
		}

		/**
		 * @brief 派生クラス用の更新処理
		*/
		void CAICar::UpdateSub()
		{
			//if (g_pad[0]->IsTrigger(enButtonX))
			//{
			//	PathFinding(g_camera3D->GetTarget());
			//}
			//return;

			if (IsEndPathMove() != true)
			{
				MoveOnPath(5000.0f, PhysicsWorld::GetInstance());
			}
			else
			{
				RandomTargetPathFinding();
			}
			return;
		}
	}
}