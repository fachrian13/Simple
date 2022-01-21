//#include <iostream>
//#include <vector>
//#include <memory>
//#include <algorithm>
//#include <bitset>
//#include <array>
//
//class Component;
//class Entity;
//
//using ComponentID = size_t;
//
//inline ComponentID GetComponentID()
//{
//	ComponentID lastId = 0;
//
//	return lastId++;
//}
//
//template<class T>
//inline ComponentID GetComponentTypeID() noexcept
//{
//	static ComponentID typeID = GetComponentID();
//
//	return typeID;
//}
//
//constexpr size_t MaxComponents = 32;
//
//using ComponentBitSet = std::bitset<MaxComponents>;
//using ComponentArray = std::array<Component*, MaxComponents>;
//
//class Component
//{
//public:
//	Entity* Entity;
//
//	virtual void Init() {}
//	virtual void Draw() {}
//	virtual void Update() {}
//
//	virtual ~Component() {}
//};
//
//class Entity
//{
//private:
//	bool Active = true;
//	std::vector<std::unique_ptr<Component>> Components;
//	ComponentArray ComponentArray;
//	ComponentBitSet ComponentBitSet;
//
//public:
//	void Update()
//	{
//		for (const auto& i : this->Components)
//			i->Update();
//
//		for (const auto& i : this->Components)
//			i->Draw();
//	}
//
//	void Draw() {}
//
//	bool IsActive()
//	{
//		return this->Active;
//	}
//
//	void Destroy()
//	{
//		this->Active = false;
//	}
//
//	//template<class T>
//	//bool HasComponent() const
//	//{
//	//	return this->ComponentBitSet[GetComponentID<T>];
//	//}
//
//	template<class T, class... TArgs>
//	Component& AddComponent(TArgs&&... component)
//	{
//		T* temp(new T(std::forward<TArgs>(component)...));
//		temp->Entity = this;
//		std::unique_ptr<Component> uPtr{ temp };
//		this->Components.emplace_back(std::move(uPtr));
//		this->ComponentArray[GetComponentTypeID<T>()] = temp;
//		this->ComponentBitSet[GetComponentTypeID<T>()] = true;
//		temp->Init();
//		return *temp;
//	}
//
//	template<class T>
//	T& GetComponent() const
//	{
//		auto ptr(this->ComponentArray[GetComponentTypeID<T>()]);
//
//		return *static_cast<T*>(ptr);
//	}
//};
//
//class Manager
//{
//private:
//	std::vector<std::unique_ptr<Entity>> Entities;
//
//public:
//	void Update()
//	{
//		for (const auto& i : this->Entities)
//			i->Update();
//	}
//
//	void Draw()
//	{
//		for (const auto& i : this->Entities)
//			i->Draw();
//	}
//
//	void Refresh()
//	{
//		this->Entities.erase(std::remove_if(this->Entities.begin(), this->Entities.end(), [](const std::unique_ptr<Entity>& entity) { return !entity->IsActive(); }), this->Entities.end());
//	}
//
//	Entity& AddEntity()
//	{
//		Entity* temp = new Entity;
//		std::unique_ptr<Entity> uPtr{ temp };
//
//		this->Entities.emplace_back(std::move(uPtr));
//		return *temp;
//	}
//};
//
//class Position : public Component
//{
//private:
//	int X = 0;
//	int Y = 0;
//
//public:
//	int XPos()
//	{
//		return this->X;
//	}
//
//	int YPos()
//	{
//		return this->Y;
//	}
//
//	void Init() override
//	{
//		this->X = 0;
//		this->Y = 0;
//	}
//
//	void Update() override
//	{
//		this->X++;
//		this->Y++;
//	}
//
//	void SetPosition(int x, int y)
//	{
//		this->X = x;
//		this->Y = y;
//	}
//
//	void Draw() override
//	{
//		std::cout << "X: " << this->X << "Y: " << this->Y << "\n";
//	}
//};
//
//class Word : public Component
//{
//
//};
//
//Manager manager;
//
//auto& newPlayer(manager.AddEntity());
//auto& newPlayer1(manager.AddEntity());
//
//int main()
//{
//	Position a, b;
//	a.SetPosition(25, 25);
//	b.SetPosition(45, 45);
//
//
//	newPlayer.AddComponent<Position>(a);
//	newPlayer.AddComponent<Position>(b);
//
//	//manager.Update();
//	manager.Draw();
//}

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Entity;
class Component;

class Position;

using ComponentTypeId = size_t;

inline ComponentTypeId GetUniqueComponentId()
{
	static ComponentTypeId lastId = 0u;

	return lastId++;
}

template<class T>
inline ComponentTypeId GetComponentTypeId() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "Type not base on Component!");

	static const ComponentTypeId typeId = GetUniqueComponentId();

	return typeId;
}

constexpr size_t MaxEntities = 5000;
constexpr size_t MaxComponents = 100;

using ComponentBitSet = std::bitset<MaxComponents>;
using ComponentList = std::array<Component*, MaxComponents>;

class Entity
{
private:
	bool Active;
	ComponentList ComponentList;
	ComponentBitSet ComponentBitSet;
	std::vector<std::unique_ptr<Component>> Components;

public:
	template<class T, class... TArgs>
	inline T& AddComponent(TArgs&&... args)
	{
		T* temp(new T(std::forward<TArgs>(args)...));
		std::unique_ptr<Component> uPtr{ temp };

		this->Components.emplace_back(std::move(uPtr));
		
		if (temp->Init())
		{
			this->ComponentList[GetComponentTypeId<T>()] = temp;
			this->ComponentBitSet[GetComponentTypeId<T>()] = true;
			temp->Entity = this;
			return *temp;
		}

		return *static_cast<T*>(nullptr);
	}

	template<class T>
	inline T& GetComponent() const
	{
		auto ptr(this->ComponentList[GetComponentTypeId<T>()]);

		return *static_cast<T*>(ptr);
	}

	template<class T>
	inline bool HasComponent() const
	{
		return this->ComponentBitSet[GetComponentTypeId<T>()];
	}

	inline bool IsActive() const
	{
		return this->Active;
	}

	inline void Destroy()
	{
		this->Active = false;
	}

	void Draw()
	{
		for (const auto& i : this->Components)
			i->Draw();
	}
};

class Component
{
public:
	Entity* Entity;

	virtual ~Component() {}

	virtual bool Init() { return true; }

	virtual void Draw() {}

	virtual void Update() {}
};

class Position : public Component
{
private:
	int X;
	int Y;

public:
	Position()
	{
		this->X = 0;
		this->Y = 0;
	}

	Position(int x, int y)
	{
		this->X = x;
		this->Y = y;
	}

	void Update() override
	{
		this->X++;
		this->Y++;
	}

	void Draw() override
	{
		std::cout << this->X << " " << this->Y;
	}

	virtual ~Position() = default;

	int GetX()
	{
		return this->X;
	}

	int GetY()
	{
		return this->Y;
	}
};

int main()
{
	Entity* entity = new Entity;

	entity->AddComponent<Position>(5, 9);
	return 0;
}