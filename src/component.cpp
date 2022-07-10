#include <ecs.hpp>

namespace Vin::Component {

#pragma region ComponentArray

	ComponentArray::ComponentArray(size_t stride) : stride{ stride }, capacity{ MIN_COMPONENTARRAY_CAPACITY }, count{ 0 } {
		data = _aligned_malloc(stride * capacity, 16);
	}

	ComponentArray::ComponentArray(size_t stride, size_t capacity) : stride{ stride }, capacity{ capacity }, count{ 0 } {
		if (capacity < MIN_COMPONENTARRAY_CAPACITY)
			capacity = MIN_COMPONENTARRAY_CAPACITY;
		data = _aligned_malloc(stride * capacity, 16);
	}

	ComponentArray::~ComponentArray() {
		_aligned_free(data);
	}

	void ComponentArray::Add(void* componentdata)
	{
		if (count >= capacity) {
			capacity *= 2;
			data = _aligned_realloc(data, stride * capacity, 16);
		}

		memcpy((char*)data + (stride * count), componentdata, stride);
		count++;
	}

	void ComponentArray::Remove(size_t idx)
	{
		if (idx >= count)
			return;

		if (idx < count - 1)
			memcpy((char*)data + (stride * idx), 
				(char*)data + (stride * (idx + 1)), 
				stride * (count - idx - 1));
		count--;
		CheckForRealloc();
	}

	size_t ComponentArray::Count()
	{
		return count;
	}

	void* ComponentArray::GetComponent(size_t idx)
	{
		if (idx >= count)
			return nullptr;
		return (char*)data + (stride * idx);
	}

	void ComponentArray::CheckForRealloc()
	{
		if (count < capacity / 2 && capacity / 2 > MIN_COMPONENTARRAY_CAPACITY) {
			capacity /= 2;
			data = _aligned_realloc(data, stride * capacity, 16);
		}
	}

#pragma endregion

	namespace {
		std::vector<ComponentArrayData> _componentArrayDatas;
	}

	std::shared_ptr<ComponentArray> FindComponentArray(Trait::IdType componentId)
	{
		auto it = std::find_if(_componentArrayDatas.begin(), _componentArrayDatas.end(), [componentId](const ComponentArrayData& v) {
			return v.componentId == componentId;
			});

		if (it == _componentArrayDatas.end())
			return nullptr;

		return it->componentArray;
	}

	void CreateComponentArray(Trait::IdType componentId, size_t componentSize)
	{
		ComponentArrayData cad{};
		cad.componentId = componentId;
		cad.componentArray = std::shared_ptr<ComponentArray>{ new ComponentArray{ componentSize } };

		_componentArrayDatas.push_back(cad);
	}

	std::shared_ptr<ComponentArray> FindOrCreateComponentArray(Trait::IdType componentId, size_t componentSize)
	{
		if (std::shared_ptr<ComponentArray> r = FindComponentArray(componentId))
			return r;
		
		ComponentArrayData cad{};
		cad.componentId = componentId;
		cad.componentArray = std::shared_ptr<ComponentArray>{ new ComponentArray{ componentSize } };

		_componentArrayDatas.push_back(cad);

		return cad.componentArray;
	}

}
