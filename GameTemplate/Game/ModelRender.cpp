#include "stdafx.h"
#include "ModelRender.h"
#include "GameTime.h"
#include "LightManager.h"
#include "RenderingEngine.h"
#include "RenderingEngineConstData.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief モデル関連のネームスペース
		*/
		namespace nsModel
		{
			// モデルの定数データを使用可能にする
			using namespace nsModelConstData;

			/**
			 * @brief 破棄されたときの処理
			*/
			void CModelRender::OnDestroy()
			{

				return;
			}

			/**
			 * @brief 常に呼ばれるアップデート関数
			*/
			void CModelRender::AlwaysUpdate()
			{
				// 初期化済みか？
				if (m_isInited != true)
					return;	// 初期化していない、早期リターン

				// スケルトンが初期化されているか？
				if (m_skeletonPtr)
				{
					// スケルトンが初期化されていたら、スケルトンを更新。

					if (m_isEnableInstancingDraw)
					{
						// インスタンシング描画時
						// 各インスタンスのワールド空間への変換は、
						// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
						// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
						m_skeletonPtr->Update(g_matIdentity);
					}
					else
					{
						// 通常描画時
						m_skeletonPtr->Update(m_model->GetWorldMatrix());
					}
				}
				// アニメーションが初期化されているか？
				if (m_animationPtr)	// アニメーションが初期化されていたら、アニメーションを進める。
					m_animationPtr->Progress(nsTimer::GameTime().GetFrameDeltaTime() * m_animationSpeed);

				// シャドウマップ描画用モデルが初期化されているか？
				if (m_shadowModels[0][0])
				{
					// 初期化されていたらモデルの座標更新
					for (auto& shadowModels : m_shadowModels)
					{
						for (auto& shadowModel : shadowModels)
						{
							shadowModel->UpdateWorldMatrix(m_position, m_rotation, m_scale);
						}
					}
				}

				// モデルの座標更新
				m_model->UpdateWorldMatrix(m_position, m_rotation, m_scale);

				return;
			}

			/**
			 * @brief 描画オブジェクト登録の入口
			*/
			void CModelRender::AddRenderEntrance()
			{
				// 初期化済みか？
				if (m_isInited != true)
					return;	// 初期化していない、早期リターン

				// 描画オブジェクトに登録する
				nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);

				return;
			}


			/**
			 * @brief 初期化関数
			 * @param[in] filePath モデルのファイルパス
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClip アニメーションクリップの数
			 * @param[in] インスタンスの最大数
			 * @param[in] modelUpAxis モデルのUP軸
			*/
			void CModelRender::Init(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis
			)
			{
				// モデルの初期化データの共通部分の設定
				SetCommonModelInitData(filePath, maxInstance, modelUpAxis);

				// GBufferのカラーフォーマットに合わせる
				for (int i = 0; i < nsMyEngine::nsRenderingEngineConstData::enGBufferNum; i++)
				{
					m_modelInitData.m_colorBufferFormat[i] = 
						nsMyEngine::CRenderingEngine::GetInstance()->GetGBufferColorFormat(
						static_cast<nsMyEngine::nsRenderingEngineConstData::EnGBuffer>(i)
					);
				}

				// GBufferに描画するモデルの定数バッファをセット
				SetRenderToGBufferModelCB();

				// GBufferに描画するモデルのシェーダーリソースビューをセット
				SetRenderToGBufferShaderResourceView();

				// 初期化処理のメインコア
				InitMainCore(animationClips, numAnimationClip);

				return;
			}
			/**
			 * @brief 半透明描画用の初期化関数
			 * @param[in] filePath モデルのファイルパス
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClip アニメーションクリップの数
			 * @param[in] インスタンスの最大数
			 * @param[in] modelUpAxis モデルのUP軸
			*/
			void CModelRender::IniTranslucent(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis
			)
			{

				// モデルの初期化データの共通部分の設定
				// 半透明描画用のシェーダーを指定する
				SetCommonModelInitData(filePath, maxInstance, modelUpAxis, kTranslucentModelFxFilePath);

				// メインレンダリングターゲットのからフォーマットに合わせる
				m_modelInitData.m_colorBufferFormat[0] =
					nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

				// 半透明描画用モデルの定数バッファをセット
				SetTranslucentModelCB();

				// 半透明描画用モデルのシェーダーリソースビューをセット
				SetTranslucentModelShaderResourceView();

				// 初期化処理のメインコア
				// ディファ―ドではなく、フォワードレンダリングで描画するように指定する
				InitMainCore(animationClips, numAnimationClip, false);

				return;
			}

			/**
			 * @brief フォワードレンダリング用の初期化関数。特殊なシェーディングを行いたいとき用。
			 * @param[in] modelInitData モデルの初期化データ
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClip アニメーションクリップの数
			*/
			void CModelRender::InitForwardRendering(
				ModelInitData& modelInitData,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// モデルの初期化データを保持
				m_modelInitData = modelInitData;

				// 初期化処理のメインコア
				// ディファ―ドではなく、フォワードレンダリングで描画するように指定する
				InitMainCore(animationClips, numAnimationClip, false);

				return;
			}

			/**
			 * @brief 初期化処理のメインコア
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClips アニメーションクリップの数
			 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
			*/
			void CModelRender::InitMainCore(
				AnimationClip* animationClips,
				const int numAnimationClips,
				const bool isDefferdRender
			)
			{
				// スケルトンの初期化
				InitSkelton();
				// スケルトンが存在するか？
				if (m_skeletonPtr)
					// 存在する。スケルトンの設定
					m_modelInitData.m_skeleton = m_skeletonPtr.get();

				// モデルの初期化
				m_model.reset(new Model);
				m_model->Init(m_modelInitData);
				// モデルの座標更新
				m_model->UpdateWorldMatrix(m_position, m_rotation, m_scale);

				// アニメーションの初期化
				InitAnimation(animationClips, numAnimationClips);

				// レンダラーの初期化
				InitRender(isDefferdRender);

				// 初期化完了
				m_isInited = true;

				return;
			}


			/**
			 * @brief スケルトンの初期化
			*/
			void CModelRender::InitSkelton()
			{
				// tkmファイルをtksファイルに変換する
				std::string skeletonFilePath = m_modelInitData.m_tkmFilePath;
				// ファイルパスの文字列から、拡張子の場所を探す
				int pos = (int)skeletonFilePath.find(kTkmFileExtensinon);
				// tkmのファイルパスからtksのファイルパスに入れ替える
				skeletonFilePath.replace(pos, kFileExtensionLength, kTksFileExtensinon);

				// スケルトンのリソースの確保
				m_skeletonPtr.reset(new Skeleton);
				// スケルトンのデータを読み込み。
				bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

				// 初期化に失敗したか？
				if (isInited != true)
				{
					// 失敗したらスケルトンのリソースを開放する
					m_skeletonPtr.reset();
				}

				return;
			}

			/**
			 * @brief アニメーションの初期化
			 * @param animationClips アニメーションクリップ
			 * @param numAnimationClips アニメーションクリップの数
			*/
			void CModelRender::InitAnimation(AnimationClip* animationClips, const int numAnimationClips)
			{
				// アニメーションクリップが登録されているか
				if (animationClips == nullptr)
					return;	// されていない場合は何もしない

				// アニメーションのリソースの確保
				m_animationPtr.reset(new Animation);
				// アニメーションを初期化。
				m_animationPtr->Init(
					*m_skeletonPtr,		// アニメーションを流し込むスケルトン。
					animationClips,		// アニメーションクリップ。
					numAnimationClips	// アニメーションの数。
				);

				return;
			}

			/**
			 * @brief モデルの初期化データの共通部分の設定
			 * @param[in] tkmFilePath モデルのtkmファイルパス
			 * @param[in] maxInstance 最大インスタンス数
			 * @param[in] modelUpAxis モデルのUP軸
			 * @param[in] fxFilePath シェーダーのfxファイルパス
			*/
			void CModelRender::SetCommonModelInitData(
				const char* tkmFilePath,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis,
				const char* fxFilePath
			)
			{
				// モデルのファイルパスの設定
				m_modelInitData.m_tkmFilePath = tkmFilePath;
				// シェーダーのファイルパスの設定
				m_modelInitData.m_fxFilePath = fxFilePath;
				// モデルのUP軸の設定
				m_modelInitData.m_modelUpAxis = modelUpAxis;
				// ピクセルシェーダーのエントリーポイントの設定
				m_modelInitData.m_psEntryPointFunc = kPsEntryPointFunc;

				// インスタンシング描画用の初期化処理
				InitInstancingDraw(maxInstance);

				if (m_isEnableInstancingDraw)
				{
					// インスタンシング描画用
					
					// インスタンシング描画用の頂点シェーダーのエントリーポイントの設定
					m_modelInitData.m_vsEntryPointFunc = kVsInstancingEntryPointFunc;
					//スキンあり、インスタンシング描画用の頂点シェーダーのエントリーポイントの設定
					m_modelInitData.m_vsSkinEntryPointFunc = kVsSkinInstancingEntryPointFunc;
				}
				else
				{
					// 通常描画用
					
					// 頂点シェーダーのエントリーポイントの設定
					m_modelInitData.m_vsEntryPointFunc = kVsEntryPointFunc;
					// スキンありの頂点シェーダーのエントリーポイントの設定
					m_modelInitData.m_vsSkinEntryPointFunc = kVsSkinEntryPointFunc;
				}

				return;

			}

			/**
			 * @brief GBufferに描画するモデルの定数バッファをセット
			*/
			void CModelRender::SetRenderToGBufferModelCB()
			{
				// モデルの拡張定数バッファの設定
				m_modelInitData.m_expandConstantBuffer[0] = &m_modelExCB;
				m_modelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelExCB);

				return;
			}

			/**
			 * @brief デフォルトの定数バッファをセット
			 * @param modelInitData モデルの初期化データ
			*/
			void CModelRender::SetTranslucentModelCB()
			{
				// ライトの情報を定数バッファに設定
				m_modelInitData.m_expandConstantBuffer[0] =
					&nsLight::CLightManager::GetInstance()->GetLightData();
				m_modelInitData.m_expandConstantBufferSize[0] =
					sizeof(nsLight::CLightManager::GetInstance()->GetLightData());

				// モデルの拡張定数バッファの設定
				m_modelInitData.m_expandConstantBuffer[1] = &m_modelExCB;
				m_modelInitData.m_expandConstantBufferSize[1] = sizeof(m_modelExCB);

				// IBL用のデータを設定
				m_modelInitData.m_expandConstantBuffer[2] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB();
				m_modelInitData.m_expandConstantBufferSize[2] =
					sizeof(nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB());

				return;
			}

			/**
			 * @brief GBufferに描画するモデルのシェーダーリソースビューをセット
			*/
			void CModelRender::SetRenderToGBufferShaderResourceView()
			{
				// インスタンシング描画を行う場合は、インスタンシング描画用のデータを設定
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
				}

				return;
			}

			/**
			 * @brief 半透明描画用モデルのシェーダーリソースビューをセット
			*/
			void CModelRender::SetTranslucentModelShaderResourceView()
			{
				// シェーダーリソースビューの番号
				int SRVNo = 0;

				// インスタンシング描画を行う場合は、インスタンシング描画用のデータを設定
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[SRVNo] = &m_worldMatrixArraySB;
				}
				SRVNo++;

				// IBL用のデータを設定
				m_modelInitData.m_expandShaderResoruceView[SRVNo++] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLTexture();

				// シャドウマップのテクスチャを設定する
				for (int i = 0; i < nsLight::nsLightConstData::kMaxDirectionalLightNum; i++)
				{
					for (int areaNo = 0; areaNo < nsGraphic::nsShadow::nsShadowConstData::enShadowMapArea_num; areaNo++)
					{
						m_modelInitData.m_expandShaderResoruceView[SRVNo++] =
							&nsMyEngine::CRenderingEngine::GetInstance()->
							GetShadowMapRenderTargetTexture(i, areaNo);
					}
				}

				return;
			}

			/**
			 * @brief レンダラーを初期化する
			 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
			*/
			void CModelRender::InitRender(const bool isDefferdRender)
			{
				if (isDefferdRender)
				{
					m_render.SetOnRenderToBGuuferFunc([&](RenderContext& rc) { this->ModelRender(rc); });
				}
				else
				{
					m_render.SetOnForwardRenderFunc([&](RenderContext& rc) { this->ModelRender(rc); });
				}

				return;
			}

			/**
			 * @brief シャドウキャスターか？を設定。trueで影を生成する、falseで影を生成しない。
			 * @param[in] isShadowCaster シャドウキャスターか？
			*/
			void CModelRender::SetIsShadowCaster(const bool isShadowCaster)
			{
				// シャドウキャスターか？
				if (isShadowCaster)
				{
					// シャドウキャスター

					// シャドウマップに描画するモデルが有効か？
					if (m_shadowModels[0][0])
					{
						// 有効なら、早期リターン
						return;
					}

					// 無効なら初期化する
					InitShadowModel();
					// シャドウマップに描画する関数を設定する
					m_render.SetOnShadwMapRender(
						[&](RenderContext& rc,const int ligNo, const int shadowMapNo, const Matrix& vlpMatrix)
						{
							this->ShadowModelRender(rc, ligNo, shadowMapNo, vlpMatrix);
						}
					);
				}
				else
				{
					// シャドウキャスターではない

					// シャドウマップに描画するモデルが無効か？
					if (!m_shadowModels[0][0])
					{
						// 無効なら、早期リターン
						return;
					}

					// 有効なら破棄する
					for (auto& shadowModels : m_shadowModels)
					{
						for (auto& shadowModel : shadowModels)
						{
							shadowModel.reset();
						}
					}
					// シャドウマップに描画する関数を、何もしないようにする
					m_render.SetOnShadwMapRender(
						[&](RenderContext&, const int, const int, const Matrix&)
						{
							return;
						}
				);
				}

				return;
			}

			/**
			 * @brief インスタンシングデータの更新
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大率
			*/
			void CModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
			{
				if (m_numInstance > m_maxInstance)
				{
					MessageBoxA(nullptr, "インスタンスの数が多すぎです", "警告", MB_OK);
				}

				if (!m_isEnableInstancingDraw) {
					return;
				}

				Matrix worldMatrix = m_model->CalcWorldMatrix(pos, rot, scale);

				// インスタンシング描画を行う。
				m_worldMatrixArray[m_numInstance] = worldMatrix;

				m_numInstance++;

				return;
			}

			/**
			 * @brief シャドウマップに描画するモデルの初期化
			*/
			void CModelRender::InitShadowModel()
			{
				// シャドウモデルの初期化データ
				ModelInitData shadowModelInitData;
				// 通常のモデルの初期化データから必要な情報を取ってくる
				shadowModelInitData.m_tkmFilePath = m_modelInitData.m_tkmFilePath;
				shadowModelInitData.m_modelUpAxis = m_modelInitData.m_modelUpAxis;
				shadowModelInitData.m_vsEntryPointFunc = m_modelInitData.m_vsEntryPointFunc;
				shadowModelInitData.m_vsSkinEntryPointFunc = m_modelInitData.m_vsSkinEntryPointFunc;
				shadowModelInitData.m_skeleton = m_modelInitData.m_skeleton;
				// fxファイルパスを設定する
				shadowModelInitData.m_fxFilePath = kDrawShadowMapFxFilePath;
				// カラーフォーマットを設定する
				shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

				// インスタンシング描画を行う場合は、インスタンシング描画用のデータを設定
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
				}

				// シャドウマップ描画用のモデルを生成して初期化する
				for (auto& shadowModels : m_shadowModels)
				{
					for (auto& shadowModel : shadowModels)
					{
						shadowModel.reset(new Model);
						shadowModel->Init(shadowModelInitData);
						shadowModel->UpdateWorldMatrix(m_position, m_rotation, m_scale);
					}
				}

				return;
			}

			/**
			 * @brief インスタンシング描画用の初期化処理
			 * @param[in] maxInstance 最大インスタンス数
			*/
			void CModelRender::InitInstancingDraw(int maxInstance)
			{
				m_maxInstance = maxInstance;
				if (m_maxInstance > 1) {
					//インスタンシング描画を行うので
					//それ用のデータを構築する
					//ワールド行列の配列のメモリを確保する
					m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
					//ワールド行列をGPUに転送するためのストラクチャードバッファを確保
					m_worldMatrixArraySB.Init(
						sizeof(Matrix),
						m_maxInstance,
						nullptr
					);
					m_isEnableInstancingDraw = true;
				}
			}


			/**
			 * @brief モデルを描画する
			 * @param[in] rc レンダリングコンテキスト
			*/
			void CModelRender::ModelRender(RenderContext& rc)
			{
				if (m_isEnableInstancingDraw)
				{
					// インスタンシング描画時

					// そのうち、フラスタムカリング入れるから、その時の処理はここに入れる
					m_fixNumInstanceOnFrame = m_numInstance;
					//// ビューフラスタムに含まれているインスタンスのみ描画する。
					//for (int instanceId = 0; instanceId < m_numInstance; instanceId++) {
					//	if (m_geometryDatas[instanceId].IsInViewFrustum()) {
					//		// ビューフラスタムに含まれている。
					//		m_worldMatrixArray[m_fixNumInstanceOnFrame] = m_worldMatrixArray[instanceId];
					//		m_fixNumInstanceOnFrame++;
					//	}
					//}

					if (m_fixNumInstanceOnFrame != 0)
					{
						// ストラクチャードバッファを更新
						m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
						// モデルを描画
						m_model->Draw(rc, m_fixNumInstanceOnFrame);
					}

				}
				else
				{
					// 通常描画時
					// モデルを描画
					m_model->Draw(rc);
				}

				return;
			}

			/**
			 * @brief シャドウマップに描画するモデルを描画する
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] ligNo ライト番号
			 * @param[in] shadowMapNo シャドウマップの番号
			 * @param[in] lvpMatrix ライトビュープロジェクション行列
			*/
			void CModelRender::ShadowModelRender(
				RenderContext& rc,
				const int ligNo,
				const int shadowMapNo,
				const Matrix& lvpMatrix
			)
			{
				if (m_shadowModels[0][0])
				{
					//@todoディレクションライトの数だけシャドウモデルを作ったら重かったので、インスタンス描画を入れるまではライト1個分のみ
					if (ligNo != 0)
						return;

					// シャドウマップに描画するモデルを描画
					m_shadowModels[ligNo][shadowMapNo]->Draw(rc, Matrix::Identity, lvpMatrix, m_fixNumInstanceOnFrame);
				}

				return;
			}


		}
	}
}