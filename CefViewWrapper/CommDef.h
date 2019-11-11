#pragma once

#define HELPER_LAUNCH_APP_NAME _T("CefViewHelper.exe")

#define CLASS_SUPPORT_DELEGATE \
public:\
	class Delegate;\
	void SetDelegate(Delegate* dele){ delegate_= dele;} \
	Delegate* GetDelegate(){ return delegate_;}\
private:\
	Delegate* delegate_ = nullptr;\



#define DELEGATE_FUNC_BEGIN \
class Delegate{ \
public: 
	




#define DELEGATE_FUNC_END \
};

