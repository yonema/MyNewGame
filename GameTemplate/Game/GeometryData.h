#pragma once
#include "AABB.h"
#include "Noncopyable.h"

namespace nsMyGame
{

	// �O���錾
	namespace nsGraphic { namespace nsModel { class CModelRender; } }

	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		/**
		 * @brief �W�I���g�����
		*/
		class CGeometryData : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGeometryData() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGeometryData() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] modelRender ���f�������_���[
			 * @param[in] instanceId �C���X�^���XID
			*/
			void Init(const nsGraphic::nsModel::CModelRender* modelRender, int instanceId);

			/**
			 * @brief �W�I���g�������X�V
			*/
			void Update();

			/**
			 * @brief �r���[�t���X�^���Ɋ܂܂�Ă��邩�H�𓾂�
			 * @return �r���[�t���X�^���Ɋ܂܂�Ă��邩�H
			*/
			bool IsInViewFrustum() const
			{
				return m_isInViewFrustum;
			}

			/**
			 * @brief AABB�̃��[���h��Ԃł̍ő咸�_���擾
			 * @return AABB�̃��[���h��Ԃł̍ő咸�_
			*/
			const Vector3& GetAabbMaxPositionInWorld() const
			{
				return m_aabbMax;
			}

			/**
			 * @brief AABB�̃��[���h��Ԃł̍ŏ����_���擾
			 * @return AABB�̃��[���h��Ԃł̍ŏ����_
			*/
			const Vector3& GetAabbMinPositionInWorld() const
			{
				return m_aabbMin;
			}

			/**
			 * @brief �V���h�E�L���X�^�[�̊􉽊w�f�[�^�H���擾
			 * @return �V���h�E�L���X�^�[�̊􉽊w�f�[�^�H
			*/
			bool IsShadowCaster() const;

			/**
			 * @brief �J��������̋������擾
			 * @return �J��������̋���
			*/
			float GetDistanceFromCamera() const
			{
				return m_distanceFromCamera;
			}

		private:	// �f�[�^�����o
			bool m_isInViewFrustum = false;			//!< �r���[�t���X�^���Ɋ܂܂�Ă��邩�H
			float m_distanceFromCamera = 0.0f;		//!< �J��������̋���
			CAABB m_aabb;							//!< AABB
			Vector3 m_aabbWorldVertexPositions[8];	//!< AABB�̃��[���h��Ԃł�8���_�B
			Vector3 m_aabbMax = Vector3::MinV;		//!< AABB�̃��[���h��Ԃł̍ő咸�_
			Vector3 m_aabbMin = Vector3::MaxV;		//!< AABB�̃��[���h��Ԃł̍ŏ����_
			//!< ���̃W�I���g�����Ɗ֘A�Â��Ă��郂�f�������_���[
			const nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;
			int m_instanceId = 0;					//!< �C���X�^���XID

		};
	}
}
