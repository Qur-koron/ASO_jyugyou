#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{

public:

	TitleScene(void);
	~TitleScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	

};
