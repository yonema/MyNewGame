/*!
 * @brief	�|�C���g���C�g�ƃX�|�b�g���C�g�̃��C�e�B���O�̌v�Z�Ɏg�p����֐��w�b�_�[�B
 */

/**
 * @brief �����ɂ��e�������v�Z����
 * @param worldPos �`���̃��[���h���W
 * @param ligPos ���C�g�̃��[���h���W
 * @param ligRange ���C�g�̉e���͈�
 * @return �e����
*/
float GetAffectOfDistance(float3 worldPos, float3 ligPos, float ligRange)
{
	// �����ɂ��e�������v�Z����

	// �|�C���g���C�g����`���̍��W�ւ̋���
	float3 distance = length(worldPos - ligPos);
	// �����ƃ|�C���g���C�g�̉e���͈͂���e�������v�Z����
	float affect = 1.0f - 1.0f / ligRange * distance;

	// �e������0�ȉ���������v�Z����K�v�Ȃ�
	if (affect <= 0.0f)
		return 0.0f;

	// �e�������A���`�ȕω�����w���֐��I�ȕω���
	affect = pow(affect, 3.0f);

	return affect;
}

/**
 * @brief �p�x�ɂ��e�������v�Z����
 * @param ligDir ���C�g�̓��˕���
 * @param ligEmmisionDir ���C�g�̎ˏo����
 * @param ligAngle ���C�g�̎ˏo�p�x
 * @return �e����
*/
float GetAffectOfAngle(float3 ligDir, float3 ligEmmisionDir, float ligAngle)
{
	// ���C�g�̓��Е����Ǝˏo�����̊p�x���v�Z����B

	// ���C�g�̓��˕����Ǝˏo�����̓��ς����߂�
	float angle = dot(ligDir, ligEmmisionDir);
	// ���ς���p�x�����߂�
	angle = abs(acos(angle));

	// �p�x�ɂ��e���������߂�
	// �p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
	float affect = 1.0f - 1.0f / ligAngle * angle;

	// �e������0�ȉ���������v�Z����K�v�Ȃ�
	if (affect <= 0.0f)
		return 0.0f;

	// �e�������A���`�ȕω�����w���֐��I�ȕω���
	affect = pow(affect, 3.0f);

	return affect;
}