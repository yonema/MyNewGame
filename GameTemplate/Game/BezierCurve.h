#pragma once

namespace nsMyGame
{
	/**
	 * @brief �Ȑ��֘A�̃l�[���X�y�[�X
	*/
	namespace nsCurve
	{
		/**
		 * @brief �x�W�F�Ȑ��N���X
		*/
		class CBezierCurve
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBezierCurve() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBezierCurve() = default;

		public:		// �����o�֐�

			/**
			 * @brief ����_��ǉ�����
			 * @param[in] controlPoint ����_
			*/
			void AddControlPoint(const Vector3& controlPoint);

			/**
			 * @brief ����_���N���A����
			*/
			void ClearControlPoints();

			/**
			 * @brief �x�W�F�Ȑ����v�Z����
			 * @param[in] t �p�����[�^�i0.0f�`1.0f�j
			 * @param[out] pointOut ���W�o�͗p
			*/
			void CalcBezierCurve(const float t, Vector3* pointOut);

		private:	// �f�[�^�����o
			std::vector<Vector3> m_controlPoints;
		};

	}
}