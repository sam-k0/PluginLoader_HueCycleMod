#pragma once
// Interface class shared by the DLLs

class IYYC_Callback
{
public:
	virtual ~IYYC_Callback() {}

	virtual double getData() = 0;
	virtual void setData(const char* pluginName,const char* key, const char* value) = 0;
	virtual void trigger() {}
};
