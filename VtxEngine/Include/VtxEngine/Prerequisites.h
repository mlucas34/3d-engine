#pragma once
#include <memory>
#include <iostream>
#include <stdexcept>

namespace Vtx
{
	class SwapChain;
	class DeviceContext;
	class VertexBuffer;
	class VertexShader;
	class PixelShader;
	class ConstantBuffer;
	class IndexBuffer;
	class GraphicsEngine;
	class RenderSystem;
	class ResourceManager;
	class Resource;
	class Texture2D;
	class TextureManager;
	class Mesh;
	class MeshManager;
	class CubeMap;
	class Material;
	class Window;
	class Game;
	class Display;
	class Texture;
	class InputSystem;
	class World;
	class Entity;
	class Component;
	class TransformComponent;
	class MeshComponent;
	class CameraComponent;
	class LightComponent;
	class TerrainComponent;
	class Game;
	class PhysicsEngine;

	typedef std::shared_ptr<SwapChain> SwapChainPtr;
	typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
	typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
	typedef std::shared_ptr<VertexShader> VertexShaderPtr;
	typedef std::shared_ptr<PixelShader> PixelShaderPtr;
	typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
	typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
	typedef std::shared_ptr<Resource> ResourcePtr;
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	typedef std::shared_ptr<Mesh> MeshPtr;
	typedef std::shared_ptr<CubeMap> CubeMapPtr;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef std::shared_ptr<Texture> TexturePtr;

	enum class Key
	{
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		_0,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		Escape,
		Shift,
		Tab,
		Enter,
		Space,
		LeftMouseButton,
		RightMouseButton,
		MiddleMouseButton,
	};

	enum class CameraType
	{
		Orthogonal = 0,
		Perspective,
	};

	enum class CullMode
	{
		None = 0,
		Front,
		Back
	};

	#define VTXError(message)\
	{\
		std::wclog << "Vtx Error: " << message << '\n';\
		throw std::runtime_error("");\
	}

	#define VTXWarning(warning)\
	std::wclog << "Vtx Warning: " << warning << '\n';\

	#define VTXInfo(info)\
	std::wclog << "Vtx Info: " << info << '\n';\

}