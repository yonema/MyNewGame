#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsAI
	{
		class CPath;
		class CCell;
		class CNaviMesh;
	}

	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief �p�X��������
		*/
		class CPathFinding
		{
		private:	// private�ȍ\����
			struct CellWork 
			{
				void Init(const CCell* cell);

				const CCell* cell;
				CellWork* parentCell;	// �e�̃Z���B
				float costFromStartCell;
				Vector3 pathPoint;
				float cost;				// �ړ��R�X�g
				bool isOpend;			// �J���ꂽ�H
				bool isClosed;			// ����ꂽ�H
				bool isSmooth;			// �X���[�X�����H
			};
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPathFinding() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPathFinding() = default;

		public:		// �����o�֐�

			/**
			 * @brief �p�X�̌������������s�B
			 * @details �������[���h���w�肳��Ă���ƁA�������[���h�ɔz�u����Ă���I�u�W�F�N�g��
			 * �Փ˔�����s���A�p�X�̃X���[�W���O�����P���܂��B
			 * ���̏Փ˔���ł́AagentRadius��agentHeight�̃p�����[�^���g�p����܂��B
			 * @param path ���������p�X
			 * @param naviMesh �i�r�Q�[�V�������b�V��
			 * @param startPos �����J�n���W
			 * @param endPos �����I�����W
			 * @param physicsWorld �������[���h�B
			 * @param agentRadius AI�G�[�W�F���g�̔��a�B�G�[�W�F���g�̃J�v�Z���R���C�_�[���쐬����̂ɗ��p����܂��B
			 * @param agentHeight AI�G�[�W�F���g�̍����B�G�[�W�F���g�̃J�v�Z���R���C�_�[���쐬����̂ɗ��p����܂��B
			*/
			void Execute(
				CPath& path,
				const CNaviMesh& naviMesh,
				const Vector3& startPos,
				const Vector3& endPos,
				PhysicsWorld* physicsWorld = nullptr,
				float agentRadius = 50.0f,
				float agentHeight = 200.0f
			);

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ���̃Z���Ɉړ�����R�X�g���v�Z
			 * @param[out] totalCost 
			 * @param[out] costFromStartCell 
			 * @param[in] nextCell 
			 * @param[in] prevCell 
			 * @param[in] endCell 
			*/
			void CalcCost(
				float& totalCost,
				float& costFromStartCell,
				const CellWork* nextCell,
				const CellWork* prevCell,
				const CCell* endCell
			);

			/**
			 * @brief �X���[�W���O
			 * @param cellList 
			 * @param[in] physicsWorld 
			 * @param[in] agentRadius AI�G�[�W�F���g�̔��a�B�G�[�W�F���g�̃J�v�Z���R���C�_�[���쐬����̂ɗ��p����܂��B
			 * @param[in] agentHeight AI�G�[�W�F���g�̍����B�G�[�W�F���g�̃J�v�Z���R���C�_�[���쐬����̂ɗ��p����܂��B
			*/
			void Smoothing(
				std::list<CellWork*>& cellList,
				PhysicsWorld* physicsWorld,
				float agentRadius,
				float agentHeight
			);

			/**
			 * @brief ���C�ƃZ���̌�������
			 * @param rayStartPos ���C�̎n�_
			 * @param rayEndPos ���C�̏I�_
			 * @param  
			 * @return 
			*/
			bool IsIntercetRayToCell(Vector3 rayStartPos, Vector3 rayEndPos, CellWork*) const;
		private:	// �f�[�^�����o

			std::vector< CellWork > m_cellWork;

		};

	}
}