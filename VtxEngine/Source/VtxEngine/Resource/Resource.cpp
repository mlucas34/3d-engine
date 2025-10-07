#include <VtxEngine/Resource/Resource.h>
#include <array>

namespace Vtx {
	Resource::Resource(const wchar_t* fullPath, ResourceManager *manager) : m_fullPath(fullPath), m_manager(manager), m_facesArray()
	{

	}

	Resource::Resource(const std::array<const wchar_t *, 6> &faces, ResourceManager *manager) : m_facesArray(faces), m_manager(manager), m_fullPath(L"")
	{

	}

	Resource::~Resource()
	{

	}
}
