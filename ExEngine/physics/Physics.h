#pragma once

#include <memory>
#include "RigidBody.h"
#include "CharacterController.h"

class PhysicsWorld
{
	static PhysicsWorld* m_instance;	//唯一のインスタンス。
	std::unique_ptr<btDefaultCollisionConfiguration> 	 m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				 m_collisionDispatcher;	//!<衝突解決処理。
	std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache;	//!<ブロードフェーズ。衝突判定の枝切り。
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_constraintSolver;		//!<コンストレイントソルバー。拘束条件の解決処理。
	std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicWorld;			//!<ワールド。
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
	CPhysicsDebugDraw									 m_debugDraw;
#endif

public:
	static void CreateInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new PhysicsWorld();
		}
	}
	static PhysicsWorld* GetInstance()
	{
		return m_instance;
	}
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	
	void Update(float deltaTime);
	void DebubDrawWorld(RenderContext& rc);
	void Release();
	/*!
	* @brief	重力を設定。。
	*/
	void SetGravity(Vector3 g)
	{
		m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
	}
	/*!
	* @brief	ダイナミックワールドを取得。
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}

	/*!
	* @brief	剛体を登録。
	*/
	void AddRigidBody(RigidBody& rb)
	{
		m_dynamicWorld->addRigidBody(rb.GetBody());
	}

	
	/*!
	* @brief	剛体を破棄。
	*/
	void RemoveRigidBody(RigidBody& rb)
	{
		m_dynamicWorld->removeRigidBody(rb.GetBody());
	}
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
#if 1
	/*!
	* @brief	コリジョンオブジェクトをワールドに登録。
	*@param[in]	colliObj	コリジョンオブジェクト。
	*/
	void AddCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->addCollisionObject(&colliObj);
	}
	/*!
	* @brief	コリジョンオブジェクトをワールドから削除。
	*@param[in]	colliObj	コリジョンオブジェクト。
	*/
	void RemoveCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->removeCollisionObject(&colliObj);
	}
	
	void ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void ContactTest(
		RigidBody& rb,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
		
	void ContactTest(
		CharacterController& charaCon,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
#endif		
private:
	PhysicsWorld();
	~PhysicsWorld();
	void Init();

	// 変更。追加。
	public:
		/**
		 * @brief レイテストを実施
		 * @param[in] rayStart レイの始点
		 * @param[in] rayEnd レイの終点
		 * @param[out] hitPos 交差の格納先
		 * @return trueが戻ってきたら衝突している
		*/
		bool RayTest(const Vector3& rayStart, const Vector3& rayEnd, Vector3& hitPos) const;
		
		/**
		 * @brief 物理ワールドに対して、凸型コライダーSweepテストを行う。
		 * @param[in,out] collider コライダー
		 * @param[in] rayStart コライダーの開始座標
		 * @param[in] rayEnd コライダーの終了座標
		 * @return trueがかえってきたら当たっている
		*/
		bool ConvexSweepTest(ICollider& collider, const Vector3& rayStart, const Vector3& rayEnd) const;

		void ConvexSweepTest(
			const btConvexShape* castShape,
			const btTransform& convexFromWorld,
			const btTransform& convexToWorld,
			btCollisionWorld::ConvexResultCallback& resultCallback,
			btScalar allowedCcdPenetration = 0.0f
		) const
		{
			m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
		}
};
	
