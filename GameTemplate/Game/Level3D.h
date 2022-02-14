#pragma once
#include "Noncopyable.h"
#include "TklFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 3D�̃��x���f�[�^�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLevel3D
	{

		/**
		 * @brief ���x���I�u�W�F�N�g�f�[�^�̍\����
		*/
		struct SLevelObjectData : public nsUtil::Noncopyable
		{
			Vector3 position;		//!< ���W�B
			Quaternion rotation;	//!< ��]�B
			Vector3 scale;			//!< �g�嗦�B
			const wchar_t* name;	//!< ���O�B
			int number;

			// �ǉ��B��������B
			bool isTranslucent = false;	//!< ���������H
			int priority = nsCommonData::enPriorityFirst;	//!< �D��x
			int numMapChipReserve = 1;	//!< �}�b�v�`�b�v�̗\��
			//!< ���[�U�[��`�̃R���W��������
			EnCollisionAttr userIndex = EnCollisionAttr::enCollisionAttr_None;
			const char* lodModelFilePath = nullptr;		//!< LOD�p�̃��f���̃t�@�C���p�X
			bool shadowCaster = false;					//!< �V���h�E�L���X�^�[���H
			//!< LOD�̐؂�ւ��̋����B�f�t�H���g�ł͕��̐��B
			//! ���̐��Ȃ烂�f�������_���[��LOD�̐؂�ւ��̋������w�肵�Ȃ�
			float distanceLOD = -1.0f;
			const char* collisionModelFilePath = nullptr;	//!< �R���W�����p���f���̃t�@�C���p�X
			// �ǉ��B�����܂ŁB

			/// <summary>
			/// �����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�����ׂ�B
			/// </summary>
			/// <param name="objName">���ׂ閼�O�B</param>
			/// <returns>���O�������ꍇ��true��Ԃ��܂��B</returns>


			/**
			 * @brief �����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�����ׂ�i���C�h������j
			 * @param[in] objName ���ׂ閼�O�i���C�h������j
			 * @return ���O���������H
			*/
			bool EqualObjectName(const wchar_t* objName)
			{
				return wcscmp(objName, name) == 0;
			}

			/**
			 * @brief �����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�����ׂ�i�}���`�o�C�g������j
			 * @param[in] objName ���ׂ閼�O�i�}���`�o�C�g������j
			 * @return ���O���������H
			*/
			bool EqualObjectName(const char* objName)
			{
				wchar_t objNameW[256];
				// sizeof(objName)���ƃ|�C���^�̃T�C�Y���߂��Ă���񂾂��B
				// ���{���肭�����Ȃ��񂾂�
				mbstowcs(objNameW, objName, std::strlen(objName) + 1);
				return wcscmp(objNameW, name) == 0;
			}

			/**
			 * @brief ���O���O����v���邩���ׂ�
			 * @param[in] n ���ׂ閼�O
			 * @return ���O���O����v���邩�H
			*/
			bool ForwardMatchName(const wchar_t* n)
			{
				auto len = wcslen(n);
				auto namelen = wcslen(name);
				if (len > namelen) {
					//���O�������B�s��v�B
					return false;
				}
				return wcsncmp(n, name, len) == 0;
			}
		};

		// �O���錾
		class CMapChip;		// �}�b�v�`�b�v�N���X

		/**
		 * @brief 3D���x���N���X
		*/
		class CLevel3D
		{
		private:	// �G�C���A�X�錾
			using MapChipPtr = std::shared_ptr<CMapChip>;

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CLevel3D();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CLevel3D() = default;

		public:		// �����o�֐�

			/**
			 * @brief ���x�����������B
			 * @param[in] filePath tkl�t�@�C���̃t�@�C���p�X
			 * @param[in] hookFunc �I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g
			 * @details �t�b�N���Ȃ��Ȃ�nullptr���w�肷��΂悢�B
			 * ���̊֐��I�u�W�F�N�g��false��Ԃ��ƁA�I�u�W�F�N�g�̏�񂩂�A
			 * �ÓI�I�u�W�F�N�g��MapChip�N���X�̃C���X�^���X����������܂��B
			 * �I�u�W�F�N�g�̖��O�ȂǂŁA�h�A��W�����v��A�A�C�e���Ȃǂ̓���ȃN���X�̃C���X�^���X��
			 * ���������Ƃ��ɁA�f�t�H���g�ō쐬�����MapChip�N���X�̃C���X�^���X���s�v�ȏꍇ��true��
			 * �Ԃ��Ă��������B
			 * �Ⴆ�΁A�t�b�N�֐��̒��ŁA�n���ꂽ�I�u�W�F�N�g�f�[�^�̖��O�̃��f����`�悷��N���X��
			 * �C���X�^���X�𐶐������Ƃ��ɁAfalse��Ԃ��Ă��܂��ƁA�������f������`�悳��邱�ƂɂȂ�܂��B
			*/
			void Init(
				const char* filePath,
				std::function<bool(SLevelObjectData& objData)> hookFunc
			);

			/**
			 * @brief tkl�t�@�C��������f�B���N�g���̃p�X��ݒ�
			 * @param[in] directoryPaht �f�B���N�g���̃p�X
			*/
			void SetDirectoryPath(const wchar_t* directoryPaht)
			{
				// ������p�X���N���A
				memset(m_directoryPath, 0, sizeof(m_directoryPath));
				// �V�����p�X���Z�b�g
				swprintf_s(m_directoryPath, directoryPaht);
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �}�b�v�`�b�v���쐬�B
			 * @param[in] objData ���x���I�u�W�F�N�g�f�[�^
			 * @param[in] filePath �t�@�C���p�X
			*/
			void CreateMapChip(const SLevelObjectData& objData, const char* filePath);

			/**
			 * @brief Tkl�t�@�C���̍s���ϊ�����B
			*/
			void MatrixTklToLevel();

		private:	// �萔
			static const int m_kMaxPathSize = 256;			//!< �t�@�C���p�X�̍ő�T�C�Y
			static const wchar_t* m_kDefaultDirectoryPath;	//!< �f�t�H���g�̃f�B���N�g���̃p�X
			static const wchar_t* m_kDirectoryPathTail;		//!< �f�B���N�g���̃p�X�̖���
		public:		// public�Ȓ萔
			static const Vector3 m_kLevelObjectOffset;		//!< ���x���I�u�W�F�N�g�̃I�t�Z�b�g
		private:	// �f�[�^�����o
			using BonePtr = std::unique_ptr<Bone>;		//!< �{�[��Ptr
			std::vector<BonePtr> m_bonelist;			//!< �{�[���̃��X�g
			std::unique_ptr<Matrix[]> m_matrixlist;		//!< �s��̃��X�g
			std::map<std::string, MapChipPtr> m_mapChipPtrs;	//!< �}�b�v�`�b�v�̉ϒ��z��B
			CTklFile m_tklFile;							//!< tkl�t�@�C��
			wchar_t m_directoryPath[m_kMaxPathSize] = {};	//!< tkl�t�@�C��������f�B���N�g���̃p�X

		};

	}
}