#include "stdafx.h"
#include "ModelRender.h"
#include "GameTime.h"
#include "LightManager.h"
#include "RenderingEngine.h"

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
					// スケルトンが初期化されていたら、スケルトンを更新。
					m_skeletonPtr->Update(m_model->GetWorldMatrix());
				// アニメーションが初期化されているか？
				if (m_animationPtr)	// アニメーションが初期化されていたら、アニメーションを進める。
					m_animationPtr->Progress(nsTimer::GameTime().GetFrameDeltaTime());

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
			 * @param filePath モデルのファイルパス
			 * @param modelUpAxis モデルのUP軸
			 * @param animationClips アニメーションクリップ
			 * @param numAnimationClip アニメーションクリップの数
			*/
			void CModelRender::Init(
				const char* filePath,
				const EnModelUpAxis modelUpAxis,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// モデルの初期化データ
				ModelInitData modelInitData;

				// モデルの初期化データの共通部分の設定
				SetCommonModelInitData(&modelInitData, filePath, modelUpAxis);

				// ディファードレンダリングにはモデルに拡張定数バッファはいらない。
				// デフォルトのコンスタントバッファを設定
				//SetDefaultConstantBuffer(&modelInitData);

				// 初期化処理のメインコア
				InitMainCore(modelInitData, animationClips, numAnimationClip);

				return;
			}
			/**
			 * @brief 半透明描画用の初期化関数
			 * @param[in] filePath モデルのファイルパス
			 * @param[in] modelUpAxis モデルのUP軸
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClip アニメーションクリップの数
			*/
			void CModelRender::IniTranslucent(
				const char* filePath,
				const EnModelUpAxis modelUpAxis,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// モデルの初期化データ
				ModelInitData modelInitData;

				// モデルの初期化データの共通部分の設定
				// 半透明描画用のシェーダーを指定する
				SetCommonModelInitData(&modelInitData, filePath, modelUpAxis, kTranslucentModelFxFilePath);

				// デフォルトのコンスタントバッファを設定
				SetDefaultConstantBuffer(&modelInitData);

				// 初期化処理のメインコア
				// ディファ―ドではなく、フォワードレンダリングで描画するように指定する
				InitMainCore(modelInitData, animationClips, numAnimationClip, false);

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
				// 初期化処理のメインコア
				// ディファ―ドではなく、フォワードレンダリングで描画するように指定する
				InitMainCore(modelInitData, animationClips, numAnimationClip, false);

				return;
			}

			/**
			 * @brief 初期化処理のメインコア
			 * @param[in] modelInitData モデルの初期化データ
			 * @param[in] animationClips アニメーションクリップ
			 * @param[in] numAnimationClips アニメーションクリップの数
			 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
			*/
			void CModelRender::InitMainCore(
				ModelInitData& modelInitData,
				AnimationClip* animationClips,
				const int numAnimationClips,
				const bool isDefferdRender
			)
			{
				// tkmファイルパスを保持する
				m_tkmFilePath = modelInitData.m_tkmFilePath;

				// スケルトンの初期化
				InitSkelton();
				// スケルトンが存在するか？
				if (m_skeletonPtr)
					// 存在する。スケルトンの設定
					modelInitData.m_skeleton = m_skeletonPtr.get();

				// モデルの初期化
				m_model.reset(new Model);
				m_model->Init(modelInitData);

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
				std::string skeletonFilePath = m_tkmFilePath;
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
			 * @param[out] modelInitData モデルの初期化データ
			 * @param[in] tkmFilePath モデルのtkmファイルパス
			 * @param[in] fxFilePath シェーダーのfxファイルパス
			 * @param[in] modelUpAxis モデルのUP軸
			 * @param[in] vsEntryPointFunc 頂点シェーダーのエントリーポイント
			 * @param[in] vsSkinEntryPointFunc スキンありの頂点シェーダーのエントリーポイント
			 * @param[in] psEntryPointFunc ピクセルシェーダーのエントリーポイント
			*/
			void CModelRender::SetCommonModelInitData(
				ModelInitData* modelInitData,
				const char* tkmFilePath,
				const EnModelUpAxis modelUpAxis,
				const char* fxFilePath,
				const char* vsEntryPointFunc,
				const char* vsSkinEntryPointFunc,
				const char* psEntryPointFunc
			)
			{
				// モデルのファイルパスの設定
				modelInitData->m_tkmFilePath = tkmFilePath;

				// モデルのUP軸の設定
				modelInitData->m_modelUpAxis = modelUpAxis;

				// シェーダーのファイルパスの設定
				modelInitData->m_fxFilePath = fxFilePath;
				// 頂点シェーダーのエントリーポイントの設定
				modelInitData->m_vsEntryPointFunc = vsEntryPointFunc;
				// スキンありの頂点シェーダーのエントリーポイントの設定
				modelInitData->m_vsSkinEntryPointFunc = vsSkinEntryPointFunc;
				// ピクセルシェーダーのエントリーポイントの設定
				modelInitData->m_psEntryPointFunc = psEntryPointFunc;

				return;

			}

			/**
			 * @brief デフォルトの定数バッファをセット
			 * @param modelInitData モデルの初期化データ
			*/
			void CModelRender::SetDefaultConstantBuffer(ModelInitData* modelInitData)
			{
				modelInitData->m_expandConstantBuffer = 
					&nsLight::CLightManager::GetInstance()->GetLightData();
				modelInitData->m_expandConstantBufferSize =
					sizeof(nsLight::CLightManager::GetInstance()->GetLightData());

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
			 * @brief モデルを描画する
			 * @param[in] rc レンダリングコンテキスト
			*/
			void CModelRender::ModelRender(RenderContext& rc)
			{
				// モデルを描画
				m_model->Draw(rc);
			}


		}
	}
}