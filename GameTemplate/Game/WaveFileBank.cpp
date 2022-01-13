#include "stdafx.h"
#include "WaveFileBank.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �T�E���h
	*/
	namespace nsSound
	{

		CWaveFileBank::~CWaveFileBank()
		{
			ReleaseAll();
		}
		void CWaveFileBank::Release(int groupID)
		{
			for (auto waveFile : m_waveFileMap[groupID]) {
				waveFile.second->Release();
			}
			m_waveFileMap[groupID].clear();
		}
		void CWaveFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "�G���[", MB_OK);
			m_waveFileMap->erase(waveFile->GetFilePathHash());
		}
		void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "�G���[", MB_OK);
			m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
		}
		CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "�G���[", MB_OK);
			auto value = m_waveFileMap[groupID].find(nsUtil::CUtil::MakeHash(filePath));
			if (value != m_waveFileMap[groupID].end()) {
				return value->second;
			}
			return CWaveFilePtr();
		}


		//WNameKey�ɂ̓n�b�V���l���ۑ�����Ă��邩��A���������n�b�V���l�����
		//�K�v���Ȃ��Ȃ邽�߁A���������Ȃ�B
		//�K�v�Ɋ��������낤
		//CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const WNameKey& nameKey)
		//{
		//	TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		//	auto value = m_waveFileMap[groupID].find(nameKey.GetHashCode());
		//	if (value != m_waveFileMap[groupID].end()) {
		//		return value->second;
		//	}
		//	return CWaveFilePtr();
		//}
	}
}