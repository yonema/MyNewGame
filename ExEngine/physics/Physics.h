#pragma once

#include <memory>
#include "RigidBody.h"
#include "CharacterController.h"

class PhysicsWorld
{
	static PhysicsWorld* m_instance;	//�B��̃C���X�^���X�B
	std::unique_ptr<btDefaultCollisionConfiguration> 	 m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				 m_collisionDispatcher;	//!<�Փˉ��������B
	std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_constraintSolver;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicWorld;			//!<���[���h�B
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
	* @brief	�d�͂�ݒ�B�B
	*/
	void SetGravity(Vector3 g)
	{
		m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
	}
	/*!
	* @brief	�_�C�i�~�b�N���[���h���擾�B
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}

	/*!
	* @brief	���̂�o�^�B
	*/
	void AddRigidBody(RigidBody& rb)
	{
		m_dynamicWorld->addRigidBody(rb.GetBody());
	}

	
	/*!
	* @brief	���̂�j���B
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
	* @brief	�R���W�����I�u�W�F�N�g�����[���h�ɓo�^�B
	*@param[in]	colliObj	�R���W�����I�u�W�F�N�g�B
	*/
	void AddCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->addCollisionObject(&colliObj);
	}
	/*!
	* @brief	�R���W�����I�u�W�F�N�g�����[���h����폜�B
	*@param[in]	colliObj	�R���W�����I�u�W�F�N�g�B
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

	// �ύX�B�ǉ��B
	public:
		/**
		 * @brief ���C�e�X�g�����{
		 * @param[in] rayStart ���C�̎n�_
		 * @param[in] rayEnd ���C�̏I�_
		 * @param[out] hitPos �����̊i�[��
		 * @return true���߂��Ă�����Փ˂��Ă���
		*/
		bool RayTest(const Vector3& rayStart, const Vector3& rayEnd, Vector3& hitPos) const;
		
		/**
		 * @brief �������[���h�ɑ΂��āA�ʌ^�R���C�_�[Sweep�e�X�g���s���B
		 * @param[in,out] collider �R���C�_�[
		 * @param[in] rayStart �R���C�_�[�̊J�n���W
		 * @param[in] rayEnd �R���C�_�[�̏I�����W
		 * @return true���������Ă����瓖�����Ă���
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
	
