#pragma once
#include <string>
#include <VtxEngine/Prerequisites.h>
#include <array>

namespace Vtx {
	class Resource
	{
	public:
		Resource(const wchar_t* fullPath, ResourceManager *manager);
		Resource(const std::array<const wchar_t *, 6> &faces, ResourceManager *manager);
		virtual ~Resource();

	protected:
		std::wstring m_fullPath;
		const std::array<const wchar_t *, 6> m_facesArray;
		ResourceManager *m_manager = nullptr;
	};
}

