#include "stdafx.h"
#include "ExEngine.h"
#include "Physics.h"

using namespace std;

PhysicsWorld* PhysicsWorld::m_instance = nullptr;	//唯一のインスタンス。

namespace {
struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
	using ContantTestCallback = function<void(const btCollisionObject& contactCollisionObject)>;
	ContantTestCallback  m_cb;
	btCollisionObject* m_me = nullptr;
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		if (m_me == colObj0Wrap->getCollisionObject()) {
			m_cb(*colObj1Wrap->getCollisionObject());
		}
		return 0.0f;
	}
};

// 変更。追加。
struct MyRayResultCallback : public btCollisionWorld::RayResultCallback
{
	Vector3 hitPos;
	Vector3 rayStart;
	Vector3 rayEnd;
	bool isHit = false;
	float hitFraction = 1.0f;
	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override
	{
		if (rayResult.m_hitFraction < hitFraction) {
			// こちらの方が近い。
			hitPos.Lerp(rayResult.m_hitFraction, rayStart, rayEnd);
		}
		isHit = true;
		return rayResult.m_hitFraction;
	}
};

struct ResultConvexSweepTest : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) override
	{
		isHit = true;
		return 0.0f;
	}
};

}

PhysicsWorld::PhysicsWorld()
{
	MY_ASSERT(
		m_instance == nullptr, 
		"PhysisWorldのインスタンスを複数作ることはできません。"
	);
	Init();
}
PhysicsWorld::~PhysicsWorld()
{
	Release();
	m_instance = nullptr;
}
void PhysicsWorld::Release()
{
	m_dynamicWorld.reset();
	m_constraintSolver.reset();
	m_constraintSolver.reset();
	m_overlappingPairCache.reset();
	m_collisionDispatcher.reset();
	m_collisionConfig.reset();
}
void PhysicsWorld::Init()
{
	Release();
	//物理エンジンを初期化。
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfig = make_unique<btDefaultCollisionConfiguration>();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConfig.get());

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = make_unique<btDbvtBroadphase>();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_constraintSolver = make_unique<btSequentialImpulseConstraintSolver>();

	m_dynamicWorld = make_unique<btDiscreteDynamicsWorld>(
		m_collisionDispatcher.get(),
		m_overlappingPairCache.get(),
		m_constraintSolver.get(),
		m_collisionConfig.get()
	);

	m_dynamicWorld->setGravity(btVector3(0, -100, 0));
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
	m_debugDraw.Init();
	m_dynamicWorld->setDebugDrawer(&m_debugDraw);
#endif 
}
void PhysicsWorld::Update(float deltaTime)
{
	m_dynamicWorld->stepSimulation(deltaTime);
}
#if 1
void PhysicsWorld::DebubDrawWorld(RenderContext& rc)
{
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
	m_debugDraw.BeginDraw(rc);
	m_dynamicWorld->debugDrawWorld();
	m_debugDraw.EndDraw();
#endif
}
void PhysicsWorld::ContactTest(
	btCollisionObject* colObj,
	std::function<void(const btCollisionObject& contactCollisionObject)> cb
) {
	MyContactResultCallback myContactResultCallback;
	myContactResultCallback.m_cb = cb;
	myContactResultCallback.m_me = colObj;
	m_dynamicWorld->contactTest(colObj, myContactResultCallback);
}

void PhysicsWorld::ContactTest(
	RigidBody& rb,
	std::function<void(const btCollisionObject& contactCollisionObject)> cb
)
{
	ContactTest(rb.GetBody(), cb);
}
void PhysicsWorld::ContactTest(
	CharacterController& charaCon,
	std::function<void(const btCollisionObject& contactCollisionObject)> cb
)
{
	ContactTest(*charaCon.GetRigidBody(), cb);
}
#endif


// 変更。追加。
bool PhysicsWorld::RayTest(const Vector3& rayStart, const Vector3& rayEnd, Vector3& hitPos) const
{
	btVector3 start, end;
	start.setValue(rayStart.x, rayStart.y, rayStart.z);
	end.setValue(rayEnd.x, rayEnd.y, rayEnd.z);
	MyRayResultCallback cb;
	cb.rayStart = rayStart;
	cb.rayEnd = rayEnd;
	m_dynamicWorld->rayTest(start, end, cb);
	if (cb.isHit) {
		hitPos = cb.hitPos;
	}
	return cb.isHit;
}

/**
 * @brief 物理ワールドに対して、凸型コライダーSweepテストを行う。
 * @param[in,out] collider コライダー
 * @param[in] rayStart コライダーの開始座標
 * @param[in] rayEnd コライダーの終了座標
 * @return trueがかえってきたら当たっている
*/
bool PhysicsWorld::ConvexSweepTest(ICollider& collider, const Vector3& rayStart, const Vector3& rayEnd) const
{
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	start.setOrigin(btVector3(rayStart.x, rayStart.y, rayStart.z));
	end.setOrigin(btVector3(rayEnd.x, rayEnd.y, rayEnd.z));
	ResultConvexSweepTest result;
	ConvexSweepTest(
		(const btConvexShape*)collider.GetBody(),
		start,
		end,
		result
	);
	return result.isHit;
}