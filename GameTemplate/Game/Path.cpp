#include "stdafx.h"
#include "Path.h"

namespace nsNinjaAttract
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief �p�X����ړ�����
		 * @details �������[���h���w�肳��Ă���ƁA�p�X�ړ���ɒn�ʂɃ��C�L���X�g���s���A
		 * ���W��n�ʂɃX�i�b�v���܂��B
		 * @param[in] pos �ړ���������W
		 * @param[in] moveSpeed �ړ����x
		 * @param[out] isEnd �p�X�ړ��I��������true���ݒ肳���
		 * @param[in] physicsWorld �������[���h�B
		 * @return �ړ���̍��W
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
				// �p�X���\�z����Ă��Ȃ�
				return pos;
			}
			SSection& currentSection = m_sectionArray.at(m_sectionNo);
			// �Z�N�V�����̏I�_�Ɍ������x�N�g�����v�Z����B
			Vector3 toEnd = currentSection.endPos - pos;
			toEnd.Normalize();
			pos += toEnd * moveSpeed;

			Vector3 toEnd2 = currentSection.endPos - pos;
			toEnd2.Normalize();

			// �ǉ�
			Vector3 distVec = currentSection.endPos - pos;
			if (physicsWorld)
			{
				distVec.y = 0.0f;
			}

			m_currentSectionTag = currentSection.sectionTag;

			if (toEnd.Dot(toEnd2) <= 0.0f || 
				distVec.LengthSq() <= 0.001f)
			{
				// �������ς�����̂ŏI�_�𒴂����B
				pos = currentSection.endPos;
				if (m_sectionNo == m_sectionArray.size() - 1) {
					// �I�_
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
				// ������Ə�B
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
		 * @brief �p�X���\�z
		*/
		void CPath::Build()
		{
			m_sectionArray.resize(m_pointArray.size() - 1);
			// �Z�N�V�������\�z���Ă����B
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