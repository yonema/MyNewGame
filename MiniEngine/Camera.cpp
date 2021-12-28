/*!
* @brief	カメラ
*/
#include "stdafx.h"
#include "Camera.h"



void Camera::Update()
{
	//アスペクト比を計算する。
	m_aspect = (float)g_graphicsEngine->GetFrameBufferWidth() / (float)g_graphicsEngine->GetFrameBufferHeight();
	if(m_isNeedUpdateProjectionMatrix){
		if (m_updateProjMatrixFunc == enUpdateProjMatrixFunc_Perspective) {
			//透視変換行列を計算。
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				m_aspect,
				m_near,
				m_far
			);
		}
		else {
			//平行投影行列を計算。
			m_projectionMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
		}
	}
	//ビュー行列の算出
	m_viewMatrix.MakeLookAt( m_position, m_target, m_up );
	//ビュープロジェクション行列の作成。
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	//ビュー行列の逆行列を計算。
	m_viewMatrixInv.Inverse( m_viewMatrix );

	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	m_right.Set(m_viewMatrixInv.m[0][0], m_viewMatrixInv.m[0][1], m_viewMatrixInv.m[0][2]);
	//カメラの回転行列を取得。
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
		// カメラより後ろに対象があったら、falseを戻す。
		return false;
	}
	return true;
}

/// <summary>
/// ワールド座標からスクリーン座標を計算する。
/// カメラより後ろにいたらisCameraFrontがfalseになる。
/// カメラの範囲外に居たらisOnFrameがtrueになって、座標が枠上になる。
/// </summary>
/// <remarks>
/// 計算されるスクリーン座標は画面の中心を{0,0}、左上を{画面の幅*-0.5,画面の高さ*-0.5}
/// 右下を{ 画面の幅 * 0.5,画面の高さ * 0.5 }とする座標系です。
/// </remarks>
/// <param name="screenPos">スクリーン座標の格納先</param>
/// <param name="worldPos">ワールド座標</param>
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
	//	// カメラより後ろなら、枠上計算を行わないで終了。
	//	return;
	//}

	const float half_w = (float)g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	const float half_h = (float)g_graphicsEngine->GetFrameBufferHeight() * 0.5f;

	Vector2 vert[4] =
	{
		// 左上
		{-half_w,half_h},
		// 右上
		{half_w,half_h},
		// 左下
		{-half_w,-half_h},
		// 右下
		{half_w,-half_h}
	};

	Vector2 edge[4] =
	{
		// 左上から右上。始点vert0。
		{ vert[1].x - vert[0].x, vert[1].y - vert[0].y },
		// 右上から右下。始点vert1。
		{ vert[3].x - vert[1].x, vert[3].y - vert[1].y },
		// 左下から左上。始点vert2。
		{ vert[0].x - vert[2].x, vert[0].y - vert[2].y },
		// 右下から左下。始点vert3。
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
			// 交差したら即抜ける
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
