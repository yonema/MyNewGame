#include "stdafx.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace nsMyGame
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		CLightManager* CLightManager::m_instance = nullptr;


		/**
		 * @brief コンストラクタ
		*/
		CLightManager::CLightManager()
		{
			if (m_instance != nullptr) {
				// インスタンスがすでに作られている。
				MessageBoxA(nullptr, "すでにCLightManagerは作られています。", "エラー", MB_OK);
				std::abort();
			}
			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CLightManager::~CLightManager()
		{
			//nullptrを入れておく
			m_instance = nullptr;
			return;
		}


		/**
		 * @brief アップデート関数。ゲームループで呼んでください。
		*/
		void CLightManager::Update()
		{
			// 視点更新
			m_lightData.eyePos = g_camera3D->GetPosition();

			return;
		}

		/**
		 * @brief ディレクションライトを追加する
		 * @param[in,out] light 追加するディレクションライト
		*/
		void CLightManager::AddLight(CDirectionalLight* light)
		{
			// ディレクションライトの数が最大数以上か？
			if (m_lightData.directionalLightNum >= nsLightConstData::kMaxDirectionalLightNum)
			{
				// ディレクションライトの数が多すぎる
				MessageBoxA(nullptr, "ディレクションライトの数が多すぎます", "エラー", MB_OK);
				std::abort();
				return;
			}

			//Manager（自身）のそれぞれのディレクションライトへのポインタにライトを渡す
			m_directionalLightPtrs[m_lightData.directionalLightNum] = light;

			// ライトにライトデータを渡す
			light->SetLightDataPtr(m_lightData.directionalLightData[m_lightData.directionalLightNum]);
			// ライトに制御ナンバーを渡す
			light->SetControlNumber(m_lightData.directionalLightNum);

			// ライトの数を増やす
			m_lightData.directionalLightNum++;

			return;
		}

		/**
		 * @brief ポイントライトを追加する
		 * @param[in,out] light 追加するポイントライト
		*/
		void CLightManager::AddLight(CPointLight* light)
		{
			// ポイントライトの数が最大数以上か？
			if (m_lightData.pointLightNum >= nsLightConstData::kMaxPointLightNum)
			{
				// ポイントライトの数が多すぎる
				MessageBoxA(nullptr, "ポイントライトの数が多すぎます", "エラー", MB_OK);
				std::abort();
				return;
			}

			//Manager（自身）のそれぞれのポイントライトへのポインタにライトを渡す
			m_pointLightPtrs[m_lightData.pointLightNum] = light;

			// ライトにライトデータを渡す
			light->SetLightDataPtr(m_lightData.pointLightData[m_lightData.pointLightNum]);
			// ライトに制御ナンバーを渡す
			light->SetControlNumber(m_lightData.pointLightNum);

			// ライトの数を増やす
			m_lightData.pointLightNum++;
		}

		/**
		 * @brief スポットライトを追加する
		 * @param[in,out] light 追加するスポットライト
		*/
		void CLightManager::AddLight(CSpotLight* light)
		{
			// スポットライトの数が最大数以上か？
			if (m_lightData.spotLightNum >= nsLightConstData::kMaxSpotLightNum)
			{
				// スポットライトの数が多すぎる
				MessageBoxA(nullptr, "スポットライトの数が多すぎます", "エラー", MB_OK);
				std::abort();
				return;
			}

			//Manager（自身）のそれぞれのスポットライトへのポインタにライトを渡す
			m_spotLightPtrs[m_lightData.spotLightNum] = light;

			// ライトにライトデータを渡す
			light->SetLightDataPtr(m_lightData.spotLightData[m_lightData.spotLightNum]);
			// ライトに制御ナンバーを渡す
			light->SetControlNumber(m_lightData.spotLightNum);

			// ライトの数を増やす
			m_lightData.spotLightNum++;
		}

		/**
		 * @brief ディレクションライトを破棄する
		 * @param[in] light 破棄するディレクションライト
		*/
		void CLightManager::RemoveLight(const CDirectionalLight* light)
		{
			// 破棄するライトの制御ナンバー
			const int targetControlNum = light->GetControlNumber();

			// i = 破棄するライトの制御ナンバー
			// i + 1 = 交換するライトの制御ナンバー
			// 交換するライトの制御ナンバーが現在のライトの数以下だったらループ
			for (int i = targetControlNum; i + 1 < m_lightData.directionalLightNum; i++)
			{
				// 破棄するデータを後ろに持っていくように並び替え
				// Manager（自身）のライトデータの、ディレクションライトのデータを並び替え
				std::swap<nsLightData::SDirectionalLightData>(
					m_lightData.directionalLightData[i],
					m_lightData.directionalLightData[i + 1]
					);
				// Manager（自身）のそれぞれのディレクションライトへのポインタを並び替え
				std::swap<CDirectionalLight*>(m_directionalLightPtrs[i], m_directionalLightPtrs[i + 1]);
			}

			// 破棄するライトが持ってた制御ナンバーから現在のライトの数-1以下だったらループ
			// （一番後ろは破棄するライトのため-1）
			for (int i = targetControlNum; i < m_lightData.directionalLightNum - 1; i++)
			{
				// 並び替え済みのデータm_lightData.directionalLightDataに、
				// それぞれのディレクションライトのデータを合わせる
				m_directionalLightPtrs[i]->SetLightDataPtr(m_lightData.directionalLightData[i]);
				// 制御ナンバーも並び替え
				m_directionalLightPtrs[i]->SetControlNumber(i);
			}

			//ディレクションライトの数を減らす
			m_lightData.directionalLightNum--;

			return;
		}

		/**
		 * @brief ポイントライトを破棄する
		 * @param[in] light 破棄するポイントライト
		*/
		void CLightManager::RemoveLight(const CPointLight* light)
		{
			// 破棄するライトの制御ナンバー
			const int targetControlNum = light->GetControlNumber();

			// i = 破棄するライトの制御ナンバー
			// i + 1 = 交換するライトの制御ナンバー
			// 交換するライトの制御ナンバーが現在のライトの数以下だったらループ
			for (int i = targetControlNum; i + 1 < m_lightData.pointLightNum; i++)
			{
				// 破棄するデータを後ろに持っていくように並び替え
				// Manager（自身）のライトデータの、ポイントライトのデータを並び替え
				std::swap<nsLightData::SPointLightData>(
					m_lightData.pointLightData[i],
					m_lightData.pointLightData[i + 1]
					);
				// Manager（自身）のそれぞれのポイントライトへのポインタを並び替え
				std::swap<CPointLight*>(m_pointLightPtrs[i], m_pointLightPtrs[i + 1]);
			}

			// 破棄するライトが持ってた制御ナンバーから現在のライトの数-1以下だったらループ
			// （一番後ろは破棄するライトのため-1）
			for (int i = targetControlNum; i < m_lightData.pointLightNum - 1; i++)
			{
				// 並び替え済みのデータm_lightData.pointLightDataに、
				// それぞれのポイントライトのデータを合わせる
				m_pointLightPtrs[i]->SetLightDataPtr(m_lightData.pointLightData[i]);
				// 制御ナンバーも並び替え
				m_pointLightPtrs[i]->SetControlNumber(i);
			}

			//ポイントライトの数を減らす
			m_lightData.pointLightNum--;

			return;
		}

		/**
		 * @brief スポットライトを破棄する
		 * @param[in] light 破棄するスポットライト
		*/
		void CLightManager::RemoveLight(const CSpotLight* light)
		{
			// 破棄するライトの制御ナンバー
			const int targetControlNum = light->GetControlNumber();

			// i = 破棄するライトの制御ナンバー
			// i + 1 = 交換するライトの制御ナンバー
			// 交換するライトの制御ナンバーが現在のライトの数以下だったらループ
			for (int i = targetControlNum; i + 1 < m_lightData.spotLightNum; i++)
			{
				// 破棄するデータを後ろに持っていくように並び替え
				// Manager（自身）のライトデータの、スポットライトのデータを並び替え
				std::swap<nsLightData::SSpotLightData>(
					m_lightData.spotLightData[i],
					m_lightData.spotLightData[i + 1]
					);
				// Manager（自身）のそれぞれのスポットライトへのポインタを並び替え
				std::swap<CSpotLight*>(m_spotLightPtrs[i], m_spotLightPtrs[i + 1]);
			}

			// 破棄するライトが持ってた制御ナンバーから現在のライトの数-1以下だったらループ
			// （一番後ろは破棄するライトのため-1）
			for (int i = targetControlNum; i < m_lightData.spotLightNum - 1; i++)
			{
				// 並び替え済みのデータm_lightData.spotLightDataに、
				// それぞれのスポットライトのデータを合わせる
				m_spotLightPtrs[i]->SetLightDataPtr(m_lightData.spotLightData[i]);
				// 制御ナンバーも並び替え
				m_spotLightPtrs[i]->SetControlNumber(i);
			}

			//スポットライトの数を減らす
			m_lightData.spotLightNum--;

			return;
		}

	}
}