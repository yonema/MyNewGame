/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#pragma once

#include <list>
#include <string>
#include "../../GameTemplate/Game/Noncopyable.h"
class RenderContext;

/*!
*@brief	ゲームオブジェクト。
* コピー禁止継承
*/
class IGameObject : private nsMyGame::nsUtil::Noncopyable
{
public:
	/*!
		*@brief	デストラクタ
		*/
	virtual ~IGameObject()
	{
	}
public:

	
	/*!
	*@brief	Updateの直前で呼ばれる開始処理。
	*@details
	* 
	* 本関数がtrueを返すとゲームオブジェクトの準備が完了したと判断されて</br>
	* Update関数が呼ばれ出します。trueを返して以降はStart関数は呼ばれなくなります。</br>
	* ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップなどを使って</br>
	* 適切に初期化を行ってください。
	*/
	virtual bool Start() { return true; }
	/*!
		*@brief	更新
		*/
	virtual void Update() {}
	///*!
	// *@brief	描画
	//*/
	//virtual void Render(RenderContext& renderContext)
	//{
	//	(void)renderContext;
	//}
	
public:
	/*!
	*@brief Start関数が完了した？
	*/
	bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief アクティブかどうかを判定。
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/// <summary>
	/// ゲームオブジェクトをアクティブにする。
	/// </summary>
	void Activate()
	{
		m_isActive = true;
	}
	/// <summary>
	/// ゲームオブジェクトを非アクティブにする。
	/// </summary>
	void Deactivate()
	{
		m_isActive = false;
	}
	
	/// <summary>
	/// 死亡させる。
	/// </summary>
	void Dead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// 死亡している？
	/// </summary>
	/// <returns>trueが返ってきたら死亡している</returns>
	bool IsDead() const
	{
		return m_isDead;
	}
	/// <summary>
	/// ゲームオブジェクトの名前を設定。
	/// </summary>
	/// <param name="name">名前</param>
	void SetName(const char* name)
	{
		if (name != nullptr) {
			m_name = name;
		}
	}
	/// <summary>
	/// 引数で渡された名前が、このゲームオブジェクトの名前とマッチするか判定。
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool IsMatchName(const char* name) const
	{
		if (strcmp(m_name.c_str(), name) == 0) {
			return true;
		}
		return false;
	}
public:

	//void RenderWrapper(RenderContext& renderContext)
	//{
	//	if (m_isActive && m_isStart && !m_isDead ) {
	//		Render(renderContext);
	//	}
	//}
	
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead ) {
			Update();
		}
	}
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead ) {
			if (Start()) {
				//初期化処理完了。
				m_isStart = true;
			}
		}
	}

		
	friend class CGameObjectManager;
protected:
	std::string m_name;								//ゲームオブジェクトの名前
	bool m_isStart = false;							//Startの開始フラグ。
	bool m_isDead = false;							//死亡フラグ。
	bool m_isRegistDeadList = false;				//死亡リストに積まれている。
	bool m_isNewFromGameObjectManager;	//GameObjectManagerでnewされた。
	bool m_isRegist = false;							//GameObjectManagerに登録されている？
	bool m_isActive = true;							//Activeフラグ。


//追加
public:		//オーバーライドしてほしいメンバ関数

	/**
	 * @brief ポーズ中のみ呼ばれるアップデート関数
	*/
	virtual void UpdateOnlyPaused() {};

	/**
	 * @brief ポーズ中でもポーズ中でなくても関係なく常に呼ばれるアップデート関数
	*/
	virtual void AlwaysUpdate() {};

	/**
	 * @brief 消去される時に呼ばれる処理
	 * @details CGameManager::DeleteGameObjectを呼んだときに実行される。
	 * デストラクタより前に実行される
	*/
	virtual void OnDestroy() {};

	/**
	 * @brief 描画オブジェクトを登録する入口となる関数。
	 * この関数をオーバーライドして、その中で、
	 * nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject()で
	 * 描画オブジェクトとして登録する。
	*/
	virtual void AddRenderEntrance() {};

public:		//メンバ関数

	/**
	 * @brief ポーズ中アップデートのラッパー
	*/
	void UpdateOnlyPausedWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			UpdateOnlyPaused();
		}
	}

	/**
	 * @brief 常に呼ばれるアップデートのラッパー
	*/
	void AlwaysUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AlwaysUpdate();
		}
	}

	/**
	 * @brief 消去された時に呼ばれる関数のラッパー
	*/
	void OnDestroyWrapper()
	{
		// デストロイイベントを実行する。
		m_destroyEvent();

		OnDestroy();
	}

	/**
	 * @brief 描画オブジェクト登録の入口
	*/
	void AddRenderEntranceWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AddRenderEntrance();
		}
	}

	/**
	 * @brief デストロイイベントを設定。自身が破棄されるときに呼ばれるイベント。
	 * @param[in] destroyEvent デストロイイベント。
	*/
	void SetDestroyEvent(const std::function<void(void)> destroyEvent)
	{
		m_destroyEvent = destroyEvent;
	}

private:	// データメンバ
	//!< デストロイイベント。自身が破棄されるときに呼ばれるイベント。
	std::function<void(void)> m_destroyEvent = []() {return; };
	
};
