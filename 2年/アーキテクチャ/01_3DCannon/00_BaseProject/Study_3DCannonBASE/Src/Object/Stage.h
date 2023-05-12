#pragma once

class Stage
{

public:

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId(void);

private:

	// ３ＤモデルのハンドルID
	int modelId_;

	// 背景
	int imgBack_;

};

