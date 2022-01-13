#include "stdafx.h"
#include "Path.h"

namespace nsNinjaAttract
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief パス上を移動する
		 * @details 物理ワールドが指定されていると、パス移動後に地面にレイキャストを行い、
		 * 座標を地面にスナップします。
		 * @param[in] pos 移動させる座標
		 * @param[in] moveSpeed 移動速度
		 * @param[out] isEnd パス移動終了したらtrueが設定される
		 * @param[in] physicsWorld 物理ワールド。
		 * @return 移動後の座標
		*/
		Vector3 CPath::Move(
			Vector3 pos,
			const float moveSpeed,
			bool& isEnd,
			const PhysicsWorld* physicsWorld
		) {
			if (m_sectionArray.empty()
				|| m_sectionNo >= m_sectionArray.size()
				) {
				// パスが構築されていない
				return pos;
			}
			SSection& currentSection = m_sectionArray.at(m_sectionNo);
			// セクションの終点に向かうベクトルを計算する。
			Vector3 toEnd = currentSection.endPos - pos;
			toEnd.Normalize();
			pos += toEnd * moveSpeed;

			Vector3 toEnd2 = currentSection.endPos - pos;
			toEnd2.Normalize();

			// 追加
			Vector3 distVec = currentSection.endPos - pos;
			if (physicsWorld)
			{
				distVec.y = 0.0f;
			}

			m_currentSectionTag = currentSection.sectionTag;

			if (toEnd.Dot(toEnd2) <= 0.0f || 
				distVec.LengthSq() <= 0.001f)
			{
				// 向きが変わったので終点を超えた。
				pos = currentSection.endPos;
				if (m_sectionNo == m_sectionArray.size() - 1) {
					// 終点
					isEnd = true;
					m_currentSectionTag = enNone;
				}
				else {
					m_sectionNo++;
					m_currentSectionTag = m_sectionArray.at(m_sectionNo).sectionTag;
				}
			}
			if (physicsWorld) {
				Vector3 rayStart = pos;
				// ちょっと上。
				rayStart.y += 1.0f;
				Vector3 rayEnd = rayStart;
				rayEnd.y -= 1000.0f;
				Vector3 hitPos;
				if (physicsWorld->RayTest(rayStart, rayEnd, hitPos)) {
					pos = hitPos;
				}
			}
			return pos;
		}

		/**
		 * @brief パスを構築
		*/
		void CPath::Build()
		{
			m_sectionArray.resize(m_pointArray.size() - 1);
			// セクションを構築していく。
			for (int pointNo = 0; pointNo < m_pointArray.size() - 1; pointNo++) {
				auto& section = m_sectionArray.at(pointNo);
				section.startPos = m_pointArray.at(pointNo);
				section.endPos = m_pointArray.at(pointNo + 1);
				section.direction = section.endPos - section.startPos;
				section.length = section.direction.Length();
				section.direction.Normalize();
				section.sectionTag = m_sectionTagArray.at(pointNo);
			}
			return;
		}
	}
}