#include "stdafx.h"
#include "TklFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{
		/**
		 * @brief tklファイルをロードする
		 * @param[in] filepath tklファイルパス
		*/
		void CTklFile::Load(const char* filepath)
		{
			// ファイルオープン
			FILE* file = fopen(filepath, "rb");
			if (file == nullptr) 
			{
				// オープン失敗
				MessageBoxA(nullptr, "ファイルの読み込みに失敗しました。", "error", MB_OK);
			}

			// バージョンの読み込み
			fread(&m_version, sizeof(m_version), 1, file);
			// オブジェクト数の読み込み
			fread(&m_objectCout, sizeof(m_objectCout), 1, file);

			// オブジェクトリストの大きさをオブジェクトの数分にサイズを変更する。
			m_objectlist.resize(m_objectCout);

			// ボーン情報の取得。
			// オブジェクトのデータを読み込んでいく
			for (int i = 0; i < m_objectCout; i++) {

				//名前の数の読み込み
				size_t nameCount = 0;
				fread(&nameCount, 1, 1, file);
				//ユニークポインタを作成。
				m_objectlist[i].name = std::make_unique<char[]>(nameCount + 1);
				fread(m_objectlist[i].name.get(), nameCount + 1, 1, file);
				//骨の親番号を入れる。
				fread(&m_objectlist[i].parentNo, sizeof(int), 1, file);
				//骨のバインドポーズ
				fread(&m_objectlist[i].bindPose, sizeof(m_objectlist[i].bindPose), 1, file);
				//骨のバインドポーズの逆行列。
				fread(&m_objectlist[i].invBindPose, sizeof(m_objectlist[i].invBindPose), 1, file);
				//自分の骨番号
				m_objectlist[i].no = i;
				//シャドウに関するフラグの調整１
				fread(&m_objectlist[i].shadowcasterflag, sizeof(false), 1, file);
				//シャドウに関するフラグの調整２
				fread(&m_objectlist[i].shadowreceiverflag, sizeof(false), 1, file);
				//intDataの処理
				int numInt = 0;
				fread(&numInt, sizeof(numInt), 1, file);
				for (int j = 0; j < numInt; i++) {
					int val = 0;
					fread(&val, sizeof(val), 1, file);
					m_objectlist[i].intData.push_back(val);
				}
				int numFloat = 0;
				fread(&numFloat, sizeof(numFloat), 1, file);
				//floatDataの処理
				for (int j = 0; j < numFloat; i++) {
					float val = 0;
					fread(&val, sizeof(val), 1, file);
					m_objectlist[i].floatData.push_back(val);
				}
				int numChar = 0;
				fread(&numChar, sizeof(numChar), 1, file);
				//charDataの処理
				for (int j = 0; j < numChar; i++) {
					int stringSize = 0;
					fread(&stringSize, sizeof(stringSize), 1, file);
					fread(&m_objectlist[i].charData, sizeof(m_objectlist[i].charData), 1, file);
				}
				int numVector3 = 0;
				fread(&numVector3, sizeof(numVector3), 1, file);
				//Vector3Dataの処理
				for (int j = 0; j < numVector3; i++) {
					float x, y, z;
					fread(&x, sizeof(x), 1, file);
					fread(&y, sizeof(y), 1, file);
					fread(&z, sizeof(z), 1, file);

					m_objectlist[i].Vector3Data.push_back(Vector3(x, y, z));
				}
			}

			// ファイルクローズ
			fclose(file);
		}
	}
}