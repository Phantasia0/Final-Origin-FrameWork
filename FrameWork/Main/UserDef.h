#pragma once

#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#define	to_tstring(p) to_wstring(p)
#else
#define _tstring string
#define _tcout cout
#define	to_tstring(p) to_string(p)
#endif

#define PI			3.141592f
#define TORADIAN	0.017453f	
#define	TODEGREE	57.295779f

#if defined UNICODE && defined _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS && defined _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 

#define SAFE_DELETE(p) {if(p){delete(p); (p)= nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[](p); (p)= nullptr;}}
#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = nullptr; }}
#define CHECK(hr) { assert(SUCCEEDED(hr)); }

#define MAKEGET(Type, Value)	\
	Type Get##Value(){return Value;} 
#define MAKESET(Type, Value)	\
	void Set##Value(Type val){Value = val;}
#define MAKEGETSET(Type, Value) \
	MAKEGET(Type, Value) MAKESET(Type, Value)
#define MAKEREFGET(Type, Value)	\
	Type& GetRef##Value(){return Value;}

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXMATRIX	Matrix;
typedef D3DXCOLOR	Color;
typedef D3DXQUATERNION Quaternion;

extern POINT					g_ptMouse;
extern ID3D11Device*			g_Device;
extern ID3D11DeviceContext*		g_DeviceContext;
extern IDXGISwapChain*			g_SwapChain;