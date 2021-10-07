#pragma once
#include "Noncopyable.h"

// �O���錾
class TkmFile;	// tkm�t�@�C���N���X

namespace nsMyGame
{
	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		/**
		 * @brief AABB�N���X
		*/
		class CAABB : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAABB() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CAABB() = default;

		public:		// public�ȗ񋓌^

			/**
			 * @brief AABB��8���_�̏��
			*/
			enum EnVertexInfo
			{
				enVertPos_X0Y0Z0,	//!< ������O�̏ꏊ�̒��_
				enVertPos_X1Y0Z0,	//!< �E����O�̏ꏊ�̒��_
				enVertPos_X0Y1Z0,	//!< �����O�̏ꏊ�̒��_
				enVertPos_X1Y1Z0,	//!< �E���O�̏ꏊ�̒��_
				enVertPos_X0Y0Z1,	//!< �������̏ꏊ�̒��_
				enVertPos_X1Y0Z1,	//!< �E�����̏ꏊ�̒��_
				enVertPos_X0Y1Z1,	//!< ���㉜�̏ꏊ�̒��_
				enVertPos_X1Y1Z1,	//!< �E�㉜�̏ꏊ�̒��_
				enVertNum			//!< ���_�̐�
			};

		public:		// �����o�֐�

			/**
			 * @brief AABB�̏���������
			 * @param[in] vMax �ő�l
			 * @param[in] vMin �ŏ��l
			*/
			void Init(const Vector3& vMax, const Vector3& vMin);

			/**
			 * @brief tkm�t�@�C������AABB�̏���������
			 * @param[in] tkmFile tkm�t�@�C��
			*/
			void InitFromTkmFile(const TkmFile& tkmFile);

			/**
			 * @brief AABB���\������8���_�̃��[���h��Ԃł̍��W���v�Z����
			 * @param[out] pos_out 8���_�̍��W���i�[
			 * @param[in] mWorld ���[���h�s��
			*/
			void CalcVertexPositions(Vector3 pos_out[enVertNum], const Matrix& mWorld);


		private:	// �f�[�^�����o
			Vector3 m_max;							//!< AABB���\������{�b�N�X�̍ő���W
			Vector3 m_min;							//!< AABB���\������{�b�N�X�̍ŏ����W
			Vector3 m_vertexPosition[enVertNum];	//!< AABB���\������8���_�̍��W

		};
	}

}

