#include "pch.h"
#include "DXGIDebug.h"

namespace Psycho
{
	DXGIDebug DXGIDebug::instance;
	void DXGIDebug::Enable()
	{
		GetInterface();
	}

	void DXGIDebug::GetLiveObjects()
	{
		GetInterface();
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL,(DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL|DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}

	void DXGIDebug::GetInterface()
	{
		if (!dxgiDebug)
		{
			EVALUATE_HR(DXGIGetDebugInterface1(0,IID_PPV_ARGS(dxgiDebug.GetAddressOf())),"Error Retrieving DXGI Debug Interface");
		}
	}
}