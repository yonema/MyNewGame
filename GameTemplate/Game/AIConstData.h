#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief AI関連の定数データ
		*/
		namespace nsAIConstData
		{
			//!< ナビメッシュのファイルパス
			constexpr const char* const kNaviMeshFilePath = "Assets/naviMesh/NM_Ground.tkn";
			//!< ナビメッシュブロックボリュームのファイルパス
			constexpr const char* const kNaviMeshBlockBolumeFilePath = "Assets/naviMesh/NMBB.tkm";
			//!< ナビメッシュのターゲットポイント用のレベルのファイルパス
			constexpr const char* const kNaviMeshTargetPointLevelFilePath = "Assets/naviMesh/NMTP.tkl";
		}
	}
}