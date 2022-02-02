#pragma once
#include "ModelRenderConstData.h"
#include "ModelRenderData.h"
#include "Render.h"
#include "LightConstData.h"
#include "ShadowConstDatah.h"
#include "GeometryData.h"

namespace nsNinjaAttract
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
			/**
			 * @brief モデル描画クラス
			*/
			class CModelRender : public IGameObject
			{
			private:	// エイリアス宣言
				using ModelPtr = std::unique_ptr<Model>;		// モデルクラスのユニークポインタ
				using SkeletonPtr = std::unique_ptr<Skeleton>;	// スケルトンクラスのユニークポインタ
				using AnimPtr = std::unique_ptr<Animation>;		// アニメーションクラスにユニークポインタ
				// ジオメトリデータのユニークポインタ
				using GeometryDataPtr = std::unique_ptr <nsGeometry::CGeometryData>;

			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CModelRender() = default;

				/**
				 * @brief デストラクタ
				*/
				~CModelRender() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final { return true; };

				/**
				 * @brief 破棄されたときの処理
				*/
				void OnDestroy() override final;

				/**
				 * @brief 常に呼ばれるアップデート関数
				*/
				void AlwaysUpdate() override final;

				/**
				 * @brief 描画オブジェクト登録の入口
				*/
				void AddRenderEntrance() override final;

			public:		// メンバ関数

				/**
				 * @brief 初期化関数
				 * @param[in] filePath モデルのファイルパス
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				 * @param[in] インスタンスの最大数
				 * @param[in] modelUpAxis モデルのUP軸
				 * @param[in] shadowCullFront 影モデルをフロントカリングにするか？
				*/
				void Init(
					const char* filePath,
					AnimationClip* animationClips = nullptr,
					const int numAnimationClip = 0,
					const int maxInstance = 1,
					const EnModelUpAxis modelUpAxis = EnModelUpAxis::enModelUpAxisZ,
					const bool shadowCullFront = false
					);

				/**
				 * @brief 半透明描画用の初期化関数
				 * @param[in] filePath モデルのファイルパス
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				 * @param[in] インスタンスの最大数
				 * @param[in] modelUpAxis モデルのUP軸
				*/
				void IniTranslucent(
					const char* filePath,
					AnimationClip* animationClips = nullptr,
					const int numAnimationClip = 0,
					const int maxInstance = 1,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ
				);

				/**
				 * @brief フォワードレンダリング用の初期化関数。特殊なシェーディングを行いたいとき用。
				 * @param[in] modelInitData モデルの初期化データ
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				*/
				void InitForwardRendering(
					ModelInitData& modelInitData,
					AnimationClip* animationClips = nullptr,
					int numAnimationClip = 0
				);

				/**
				 * @brief 座標を設定する
				 * @param[in] position 座標
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief 座標を得る
				 * @return[in] 座標
				*/
				const Vector3& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief 回転を設定する
				 * @param[in] rot 回転を得る
				*/
				void SetRotation(const Quaternion& rot)
				{
					m_rotation = rot;
				}

				/**
				 * @brief 回転を得る
				 * @return 回転
				*/
				const Quaternion& GetRotation() const
				{
					return m_rotation;
				}

				/**
				 * @brief 拡大率を設定する
				 * @param[in] scale 拡大率
				*/
				void SetScale(const Vector3 scale)
				{
					m_scale = scale;
				}

				/**
				 * @brief 拡大率を得る
				 * @return 拡大率
				*/
				const Vector3& GetScale() const
				{
					return m_scale;
				}

				/**
				 * @brief アニメーションを再生する
				 * @param[in] animNo アニメーションクリップの番号
				 * @param[in] interpolateTime 補完時間（単位：秒）
				*/
				void PlayAnimation(
					const int animNo,
					const float interpolateTime = nsModelConstData::kAnimationInterpolateTime
				)
				{
					m_animationPtr->Play(animNo, interpolateTime);
					return;
				}

				/**
				 * @brief アニメーションが再生中か？を得る
				 * @return アニメーションが再生中か？
				*/
				bool IsAnimationPlaying() const
				{
					return m_animationPtr->IsPlaying();
				}

				/**
				 * @brief アニメーションの残り時間（比率）を得る
				 * @return アニメーションの残り時間（比率）
				*/
				float GetAnimationTimeRemaining() const
				{
					return m_animationPtr->GetTimeRemaining();
				}

				/**
				 * @brief アニメーションのスピードを設定
				 * @param[in] animSpeed アニメーションのスピード
				*/
				void SetAnimationSpeed(const float animSpeed)
				{
					m_animationSpeed = animSpeed;
				}

				/**
				 * @brief 自己発光カラーを設定
				 * @param[in] emmisonColor 自己発光カラー
				*/
				void SetEmmisonColor(const Vector4& emmisonColor)
				{
					m_model->SetEmmisonColor(emmisonColor);
				}

				/**
				 * @brief 乗算カラーを設定
				 * @param[in] mulColor 乗算カラー
				*/
				void SetMulColor(const Vector4& mulColor)
				{
					m_model->SetMulColor(mulColor);
				}

				/**
				 * @brief モデルのアルファ値を設定
				 * @param[in] alphaValue アルファ値
				*/
				void SetAlphaValue(const float alphaValue)
				{
					m_model->SetAlphaValue(alphaValue);
				}

				/**
				 * @brief シャドウキャスターか？を設定。trueで影を生成する、falseで影を生成しない。
				 * @param[in] isShadowCaster シャドウキャスターか？
				*/
				void SetIsShadowCaster(const bool isShadowCaster);

				/**
				 * @brief シャドウレシーバーか？を設定。trueで影が映る、falseで影が映らない。
				 * @param[in] isShadowReciever シャドウレシーバーか？
				*/
				void SetIsShadowReciever(const bool isShadowReciever)
				{
					m_modelExCB.isShadowReciever = isShadowReciever;
				}

				/**
				 * @brief プレイヤー専用のシャドウキャスターを設定
				 * @param[in] isPlayerShaodwCaster プレイヤー専用のシャドウキャスターか？
				*/
				void SetIsPlayerShadowCaster(bool isPlayerShaodwCaster);

				/**
				 * @brief シャドウキャスターか？を取得
				 * @return シャドウキャスターか？
				*/
				bool IsShadowCaster() const
				{
					return m_isShadowCaster;
				}

				/**
				 * @brief インスタンシング描画を行うか？を取得
				 * @return インスタンシング描画を行うか？
				*/
				bool IsInstancingDraw() const
				{
					return m_isEnableInstancingDraw;
				}

				/**
				 * @brief モデルの参照を得る
				 * @return モデルの参照
				*/
				Model& GetModel() const
				{
					return *m_model.get();
				}

				/**
				 * @brief インスタンスIDを指定して、ワールド行列を得る
				 * @param instanceId インスタンスID
				 * @return ワールド行列
				*/
				const Matrix& GetWorldMatrix(int instanceId) const
				{
					if (IsInstancingDraw()) {
						return m_worldMatrixArray[instanceId];
					}
					return m_model->GetWorldMatrix();
				}

				/**
				 * @brief インスタンシングデータの更新
				 * @param[in] pos 座標
				 * @param[in] rot 回転
				 * @param[in] scale 拡大率
				*/
				void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

				/**
				 * @brief LOD用のモデルの初期化
				 * @param[in] filePath LOD用のモデルのファイルパス
				*/
				void InitLODModel(const char* filePath);

				/**
				 * @brief LODは有効か？を設定する
				 * @param[in] isEnableLOD LODは有効か？
				*/
				void SetIsEnableLOD(const bool isEnableLOD)
				{
					m_isEnableLOD = isEnableLOD;
				}

				/**
				 * @brief LODの距離を設定
				 * @param[in] distanceLOD LODの距離
				*/
				void SetDistanceLOD(float distanceLOD)
				{
					m_distanceLOD = distanceLOD;
				}

				/**
				 * @brief ボーンの名前からボーン番号を検索
				 * @param[in] boneName ボーンの名前
				 * @return ボーン番号。見つからなかった場合は-1が返ってきます。
				*/
				int FindBoneID(const wchar_t* boneName) const
				{
					if (m_skeletonPtr)
					{
						return m_skeletonPtr->FindBoneID(boneName);
					}
					return -1;
				}

				/**
				 * @brief ボーン番号からボーンを取得。
				 * @param[in] boneNo ボーン番号
				 * @return ボーン
				*/
				const Bone* GetBone(int boneNo) const
				{
					if (m_skeletonPtr)
					{
						return m_skeletonPtr->GetBone(boneNo);
					}
					return nullptr;
				}


			private:	// privateなメンバ関数

				/**
				 * @brief 初期化処理のメインコア
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClips アニメーションクリップの数
				 * @param[in] maxInstance インスタンス数
				 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
				*/
				void InitMainCore(
					AnimationClip* animationClips,
					const int numAnimationClips,
					const int maxInstance,
					const bool isDefferdRender
				);

				/**
				 * @brief スケルトンの初期化
				*/
				void InitSkelton();

				/**
				 * @brief アニメーションの初期化
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClips アニメーションクリップの数
				*/
				void InitAnimation(AnimationClip* animationClips, const int numAnimationClips);

				/**
				 * @brief ジオメトリ情報を初期化
				 * @param maxInstance インスタンス数
				*/
				void InitGeometryDatas(const int maxInstance);

				/**
				 * @brief モデルの初期化データの共通部分の設定
				 * @param[in] tkmFilePath モデルのtkmファイルパス
				 * @param[in] maxInstance 最大インスタンス数
				 * @param[in] fxFilePath シェーダーのfxファイルパス
				 * @param[in] modelUpAxis モデルのUP軸
				*/
				void SetCommonModelInitData(
					const char* tkmFilePath,
					const int maxInstance,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath
				);

				/**
				 * @brief GBufferに描画するモデルの定数バッファをセット
				*/
				void SetRenderToGBufferModelCB();

				/**
				 * @brief 半透明描画用モデルの定数バッファをセット
				*/
				void SetTranslucentModelCB();

				/**
				 * @brief GBufferに描画するモデルのシェーダーリソースビューをセット
				*/
				void SetRenderToGBufferShaderResourceView();

				/**
				 * @brief 半透明描画用モデルのシェーダーリソースビューをセット
				*/
				void SetTranslucentModelShaderResourceView();

				/**
				 * @brief レンダラーを初期化する
				 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
				*/
				void InitRender(const bool isDefferdRender);

				/**
				 * @brief シャドウマップに描画するモデルの初期化
				*/
				void InitShadowModel();

				/**
				 * @brief インスタンシング描画用の初期化処理
				 * @param[in] maxInstance 最大インスタンス数
				*/
				void InitInstancingDraw(int maxInstance);

				/**
				 * @brief モデルを描画する
				 * @param[in] rc レンダリングコンテキスト
				*/
				void ModelRender(RenderContext& rc);

				/**
				 * @brief シャドウマップに描画するモデルを描画する
				 * @param[in] rc レンダリングコンテキスト
				 * @param[in] ligNo ライト番号
				 * @param[in] shadowMapNo シャドウマップの番号
				 * @param[in] lvpMatrix ライトビュープロジェクション行列
				*/
				void ShadowModelRender(RenderContext& rc,const int ligNo, const int shadowMapNo, const Matrix& lvpMatrix);

				/**
				 * @brief プレイヤー専用のシャドウマップに描画するモデルを描画する
				 * @param[in] rc レンダリングコンテキスト
				 * @param[in] ligNo ライトの番号
				 * @param[in] lvpMatrix ライトビュープロジェクション行列
				*/
				void PlayerShadowModelRender(RenderContext& rc,const int ligNo, const Matrix& lvpMatrix);

				/**
				 * @brief インスタンシング描画時の描画関数
				 * @param[in] rc レンダリングコンテキスト
				*/
				void InstancingModelRender(RenderContext& rc);

			private:	// データメンバ
				ModelPtr m_model;							//!< モデルクラス
				//@todoディレクションライトの数だけシャドウモデルを作ったら重かったので、インスタンス描画を入れるまではライト1個分のみ
				//!< シャドウマップ描画用モデルクラス
				ModelPtr m_shadowModels[1/*nsLight::nsLightConstData::kMaxDirectionalLightNum*/][nsShadow::nsShadowConstData::enShadowMapArea_num];
				ModelPtr m_lodModel;						//!< LOD用のモデル
				//!< LOD用のシャドウモデル
				ModelPtr m_lodShadowModel[1/*nsLight::nsLightConstData::kMaxDirectionalLightNum*/][nsShadow::nsShadowConstData::enShadowMapArea_num];
				ModelInitData m_modelInitData;				//!< モデルの初期化データ

				CRender m_render;							//!< レンダラークラス
				SkeletonPtr m_skeletonPtr;					//!< スケルトンクラス
				AnimPtr m_animationPtr;						//!< アニメーションクラス
				float m_animationSpeed = 1.0f;				//!< アニメーションのスピード
				nsModelData::SExpandConstantBuffer m_modelExCB;	//!< モデルの拡張定数バッファ

				Vector3 m_position = Vector3::Zero;				//!< 座標
				Quaternion m_rotation = Quaternion::Identity;	//!< 回転
				Vector3 m_scale = Vector3::One;					//!< 拡大率

				int	m_numInstance = 0;						//!< インスタンスの数。
				int	m_maxInstance = 1;						//!< 最大インスタンス数。
				int	m_fixNumInstanceOnFrame = 0;			//!< このフレームに描画するインスタンスの数の確定数。
				int	m_fixNumInstanceOnFrameLOD = 0;			//!< LOD用のこのフレームに描画するインスタンスの数の確定数。
				int	m_shadowFixNumInstanceOnFrame = 0;		//!< 影用のこのフレームに描画するインスタンスの数の確定数。
				int	m_shadowFixNumInstanceOnFrameLOD = 0;	//!< 影用のLOD用のこのフレームに描画するインスタンスの数の確定数。
				std::unique_ptr<Matrix[]> m_worldMatrixArray;	//!< ワールド行列の配列。
				std::unique_ptr<Matrix[]> m_worldMatrixArrayBuffer;	//!< カリング後のワールド行列の配列。
				std::unique_ptr<Matrix[]> m_worldMatrixArrayBufferLOD;	//!< LOD用のカリング後のワールド行列の配列。
				std::unique_ptr<Matrix[]> m_shadowWorldMatrixArrayBuffer;	//!< 影モデル用カリング後のワールド行列の配列。
				std::unique_ptr<Matrix[]> m_shadowWorldMatrixArrayBufferLOD;	//!< 影モデル用LOD用のカリング後のワールド行列の配列。
				StructuredBuffer m_worldMatrixArraySB;		//!< ワールド行列の配列のストラクチャードバッファ。
				StructuredBuffer m_worldMatrixArraySBLOD;	//!< LOD用のールド行列の配列のストラクチャードバッファ。
				StructuredBuffer m_shadowWorldMatrixArraySB;	//!< 影モデル用ワールド行列の配列のストラクチャードバッファ。
				StructuredBuffer m_shadowWorldMatrixArraySBLOD;	//!< 影モデル用LOD用のールド行列の配列のストラクチャードバッファ。

				std::vector<GeometryDataPtr> m_geometryDatas;	//!< ジオメトリ情報のコンテナ
				float m_distanceLOD = nsModelConstData::kDefaultDistanceLOD;	//!< LODの距離

				bool m_isEnableInstancingDraw = false;		//!< インスタンシング描画が有効か？
				bool m_isInited = false;					//!< 初期化済みか？
				bool m_isShadowCaster = false;				//!< シャドウキャスターか？
				bool m_isEnableLOD = false;					//!< LODは有効か？
				bool m_isShadowCullFront = false;			//!< 影モデルをフロントカリングにするか？
			};

		}
	}
}