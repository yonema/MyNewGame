#include "stdafx.h"
#include "SwingTarget.h"
#include "StringActionTargetManager.h"

namespace nsMyGame
{
	/**
	 * @brief �����g�����A�N�V�����̃^�[�Q�b�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsStringActionTarget
	{
		/**
		 * @brief �X�C���O�p�I�u�W�F�N�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsSwingTarget
		{

			/**
			 * @brief �f�X�g���N�^
			*/
			CSwingTarget::~CSwingTarget()
			{
				// �}�l�[�W���[�̎��g�̓o�^����������
				CStringActionTargetManager::GetInstance()->RemoveSwingTarget(this);

				return;
			}
			/**
			 * @brief ������
			 * @param[in] modelRender ���f�������_���[
			*/
			void CSwingTarget::Init(const nsGraphic::nsModel::CModelRender& modelRender)
			{
				m_modelRender = &modelRender;

				// tkm�t�@�C������AABB��������
				m_aabb.InitFromTkmFile(*m_modelRender->GetModel().GetTkmFile());

				// �}�l�[�W���[�Ɏ��g��o�^����
				CStringActionTargetManager::GetInstance()->AddSwingTarget(this);

				return;
			}


			/**
			 * @brief �X�C���O�̃^�[�Q�b�g�B�̍��W���v�Z����
			 * derails ������ (1, 1, 1) �Ń^�[�Q�b�g�̐���8�i8���_���j
			 * @param[in] widthSegments ���Z�O�����g ( X )
			 * @param[in] heightSegments �����Z�O�����g ( Y )
			 * @param[in] lenghtSegments �����Z�O�����g ( Z )
			*/
			void CSwingTarget::CalcSwingingTargetPositions(
				const UINT widthSegments,
				const UINT heightSegments,
				const UINT lengthSegments
			)
			{
				// �W�I���g���֘A�̃l�[���X�y�[�X���g�p�\�ɂ���
				using namespace nsGeometry;

				// �R���e�i�����Z�b�g
				// �R���e�i�̃f�[�^��j��
				m_swingTargetPositions.clear();
				// �R���e�i�̃L���p�V�e�B�����Z�b�g
				m_swingTargetPositions.shrink_to_fit();


				// �^�[�Q�b�g�̐�
				int TargetsNum = 0;
				// AABB�̕ǁi���ʁj�̕����Ƀ^�[�Q�b�g�����
				// �O�ʂƌ�ʂ̃^�[�Q�b�g�̐������Z����
				TargetsNum += (widthSegments + 1) * (heightSegments + 1) * 2;
				// ���ʂƉE�ʂ̃^�[�Q�b�g�̐������Z����
				TargetsNum += (lengthSegments + 1) * (heightSegments + 1) * 2;
				// �d�����Ă���l�p�̃^�[�Q�b�g�����Z����
				TargetsNum -= (heightSegments + 1) * 4;

				// �R���e�i�̃L���p�V�e�B���^�[�Q�b�g�̐��������m�ۂ���
				m_swingTargetPositions.reserve(TargetsNum);

				// AABB��8���_�̍��W
				Vector3 vertexPos[CAABB::enVertNum];

				// AABB��8���_�̃��[���h���W���v�Z
				m_aabb.CalcVertexPositions(vertexPos, m_modelRender->GetModel().GetWorldMatrix());

				Vector3 boxWidth = vertexPos[CAABB::enVertPos_X1Y0Z0] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 boxHeight = vertexPos[CAABB::enVertPos_X0Y1Z0] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 boxLength = vertexPos[CAABB::enVertPos_X0Y0Z1] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 widhtSegmentVec = boxWidth / static_cast<float>(widthSegments);
				Vector3 heightSegmentVec = boxHeight / static_cast<float>(heightSegments);
				Vector3 lengthSegmentVec = boxLength / static_cast<float>(lengthSegments);


				for (UINT hSeg = 0; hSeg < heightSegments + 1; hSeg++)
				{
					Vector3 hSegVec = heightSegmentVec * static_cast<float>(hSeg);

					// �O�ʂƌ�ʂ̃^�[�Q�b�g�̍��W���v�Z���ĕێ�����
					for (UINT wSeg = 0; wSeg < widthSegments + 1; wSeg++)
					{
						Vector3 targetOffset = hSegVec + widhtSegmentVec * static_cast<float>(wSeg);

						// �O�ʂ̃^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z0] + targetOffset
						);
						// ��ʂ̃^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z1] + targetOffset
						);
					}

					// ���ʂƉE�ʂ̃^�[�Q�b�g�̍��W���v�Z���ĕێ�����
					for (UINT lSeg = 1; lSeg < lengthSegments; lSeg++)
					{
						Vector3 targetOffset = hSegVec + lengthSegmentVec * static_cast<float>(lSeg);

						// ���ʂƃ^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z0] + targetOffset
						);
						// �E�ʂƃ^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X1Y0Z0] + targetOffset
						);
					}
				}


				return;
			}

		}
	}
}