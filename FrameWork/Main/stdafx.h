#pragma once
#define NOMINMAX

#include <iostream> 
#include <vector>
#include <functional>
#include <iterator>
#include <map>
#include <thread>
#include <queue>
#include <fstream>
using namespace std;

#include <math.h>
#include <float.h>
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>

#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

#include "imgui.h"
#include "imguiDx11.h"
#pragma comment(lib, "imgui.lib")

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#pragma comment(lib, "assimp/assimp-vc140-mt.lib")

#pragma comment (lib,"fmod/fmodex_vc.lib")
#include "fmod/fmod.hpp"

#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

#include "UserDef.h"

#include "Singleton\Singleton.h"
#include "Singleton\D3D.h"
extern D3DDesc g_D3dDesc;
#include "Singleton\KeyManager.h"
#include "Singleton\TimeManager.h"

#include "Render\Shader.h"
#include "Render\InputLayout.h"
#include "Render\Material.h"
#include "Render\Texture.h"
#include "Render\RenderState.h"
#include "Render\RenderTarget.h"

#include "Viewer\View.h"
#include "Viewer\Viewport.h"
#include "Viewer\Projection.h"
#include "Viewer\Perspective.h"
#include "Viewer\Orthographic.h"

#include "Object\Object.h"
#include "Object\Cam.h"

#include "Singleton\CamManager.h"
#include "Singleton\ShaderManager.h"
#include "Singleton\LightManager.h"
#include "Singleton\SceneManager.h"
#include "Singleton\SoundManager.h"
#include "Singleton\WriteManager.h"

#include "Util\Collider.h"
#include "Util\Linear.h"

#include "Object\Cube.h"
#include "Object\Grid.h"
#include "Object\Sphere.h"
#include "Object\Plane.h"
#include "Object\Line.h"
#include "Object\HeightMap.h"
#include "Object\Ui.h"
#include "Object\Model\Model.h"
#include "Object\SkySphere.h"
#include "Object\Frustum.h"
#include "Object\RenderTarget\PostEffect.h"
#include "Object\RenderTarget\Blur.h"
#include "Object\Terrain\Terrain.h"
#include "Object\Terrain\QuadTree.h"
#include "Object\Terrain\TerrainEditor.h"
#include "Object\Particle\Particle.h"
#include "Object\Particle\Rain.h"
#include "Object\Shadow.h"
#include "Object\Reflection.h"
#include "Object\Refraction.h"

#include "Util\Collision.h"
#include "Util\Astar.h"
#include "Util\Dijkstra.h"
#include "Util\DiEditer.h"

#include "Scene\Scene.h"

#define CAMMANAGER CamManager::GetInstance()
#define SHADERMANAGER ShaderManager::GetInstance()
#define LIGHTMANAGER LightManager::GetInstance()
#define SCENEMANAGER SceneManager::GetInstance()
#define SOUNDMANAGER SoundManager::GetInstance()
#define WRITEMANAGER WriteManager::GetInstance()
#define D3DMANAGER D3D::GetInstance()
#define KEYMANAGER KeyManager::GetInstance()
#define TIMEMANAGER TimeManager::GetInstance()
#define DeltaTime TimeManager::GetInstance()->GetElapsedTime()
