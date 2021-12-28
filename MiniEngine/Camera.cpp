/*!
* @brief	�J����
*/
#include "stdafx.h"
#include "Camera.h"



void Camera::Update()
{
	//�A�X�y�N�g����v�Z����B
	m_aspect = (float)g_graphicsEngine->GetFrameBufferWidth() / (float)g_graphicsEngine->GetFrameBufferHeight();
	if(m_isNeedUpdateProjectionMatrix){
		if (m_updateProjMatrixFunc == enUpdateProjMatrixFunc_Perspective) {
			//�����ϊ��s����v�Z�B
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				m_aspect,
				m_near,
				m_far
			);
		}
		else {
			//���s���e�s����v�Z�B
			m_projectionMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
		}
	}
	//�r���[�s��̎Z�o
	m_viewMatrix.MakeLookAt( m_position, m_target, m_up );
	//�r���[�v���W�F�N�V�����s��̍쐬�B
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	//�r���[�s��̋t�s����v�Z�B
	m_viewMatrixInv.Inverse( m_viewMatrix );

	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	m_right.Set(m_viewMatrixInv.m[0][0], m_viewMatrixInv.m[0][1], m_viewMatrixInv.m[0][2]);
	//�J�����̉�]�s����擾�B
	m_cameraRotation = m_viewMatrixInv;
	m_cameraRotation.m[3][0] = 0.0f;
	m_cameraRotation.m[3][1] = 0.0f;
	m_cameraRotation.m[3][2] = 0.0f;
	m_cameraRotation.m[3][3] = 1.0f;

	Vector3 toPos;
	toPos.Subtract(m_position, m_target);
	m_targetToPositionLen = toPos.Length();

	m_isDirty = false;
}
void Camera::CalcScreenPositionFromWorldPosition(Vector2& screenPos, const Vector3& worldPos) const
{
	float half_w = (float)g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	float half_h = (float)g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
	Vector4 _screenPos;
	_screenPos.Set(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	m_viewProjectionMatrix.Apply(_screenPos);
	screenPos.x = (_screenPos.x / _screenPos.w)*half_w;
	screenPos.y = (_screenPos.y / _screenPos.w)*half_h;
}
bool Camera::CalcScreenPositionFromWorldPositionBackCull(Vector2& screenPos, const Vector3& worldPos) const
{
	float half_w = (float)g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	float half_h = (float)g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
	Vector4 _screenPos;
	_screenPos.Set(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	m_viewProjectionMatrix.Apply(_screenPos);
	screenPos.x = (_screenPos.x / _screenPos.w) * half_w;
	screenPos.y = (_screenPos.y / _screenPos.w) * half_h;

	if (_screenPos.z <= 0.0f)
	{
		// �J���������ɑΏۂ���������Afalse��߂��B
		return false;
	}
	return true;
}

/// <summary>
/// ���[���h���W����X�N���[�����W���v�Z����B
/// �J���������ɂ�����isCameraFront��false�ɂȂ�B
/// �J�����͈̔͊O�ɋ�����isOnFrame��true�ɂȂ��āA���W���g��ɂȂ�B
/// </summary>
/// <remarks>
/// �v�Z�����X�N���[�����W�͉�ʂ̒��S��{0,0}�A�����{��ʂ̕�*-0.5,��ʂ̍���*-0.5}
/// �E����{ ��ʂ̕� * 0.5,��ʂ̍��� * 0.5 }�Ƃ�����W�n�ł��B
/// </remarks>
/// <param name="screenPos">�X�N���[�����W�̊i�[��</param>
/// <param name="worldPos">���[���h���W</param>
void Camera::CalcScreenPositionFromWorldPositionBackCullOnFrame(
	Vector2& screenPos,
	Vector2& prevPos,
	const Vector3& worldPos,
	bool* isCameraFront,
	bool* isOnFrame,
	float onFrameBuff
) const
{
	*isCameraFront = CalcScreenPositionFromWorldPositionBackCull(screenPos, worldPos);
	prevPos = screenPos;
	//if (*isCameraFront == false)
	//{
	//	// �J���������Ȃ�A�g��v�Z���s��Ȃ��ŏI���B
	//	return;
	//}

	const float half_w = (float)g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	const float half_h = (float)g_graphicsEngine->GetFrameBufferHeight() * 0.5f;

	Vector2 vert[4] =
	{
		// ����
		{-half_w,half_h},
		// �E��
		{half_w,half_h},
		// ����
		{-half_w,-half_h},
		// �E��
		{half_w,-half_h}
	};

	Vector2 edge[4] =
	{
		// ���ォ��E��B�n�_vert0�B
		{ vert[1].x - vert[0].x, vert[1].y - vert[0].y },
		// �E�ォ��E���B�n�_vert1�B
		{ vert[3].x - vert[1].x, vert[3].y - vert[1].y },
		// �������獶��B�n�_vert2�B
		{ vert[0].x - vert[2].x, vert[0].y - vert[2].y },
		// �E�����獶���B�n�_vert3�B
		{ vert[2].x - vert[3].x, vert[2].y - vert[3].y }
	};

	bool isIntersect = false;
	Vector2 finalPos = screenPos;
	Vector2 toPos = screenPos;
	toPos.Normalize();
	toPos.x *= (1.0f + onFrameBuff);
	toPos.y *= (1.0f + onFrameBuff);
	toPos.x += screenPos.x;
	toPos.y += screenPos.y;
	for (int i = 0; i < 4; i++)
	{
		isIntersect = IsIntersectVector2ToVector2(
			Vector2::Zero,
			toPos,
			vert[i],
			edge[i],
			&finalPos
		);
		if (isIntersect)
		{
			// ���������瑦������
			screenPos = finalPos;
			*isOnFrame = true;
			break;
		}
	}

	return;
}

void Camera::RotateOriginTarget(const Quaternion& qRot)
{
	Vector3 cameraPos = m_position;
	Vector3 cameraTarget = m_target;
	Vector3 toPos = cameraPos - cameraTarget;
	qRot.Apply(toPos);
	m_position = m_target + toPos;
	m_isDirty = true;
}
