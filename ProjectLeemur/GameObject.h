#pragma once

#include "Component.h"
#include "TransformEntity.h"
#include "Mesh.h"

/* Used for type_id mapping */
using TypeRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
	std::size_t operator()(TypeRef code) const;
};

struct Comparator {
	bool operator()(TypeRef lhs, TypeRef rhs) const;
};





class GameObject : public BaseEntity {
private:
	Component * component;
	std::unordered_map<TypeRef, Component*, Hasher, Comparator> components;


	// TODO: singular component API
	UniquePointer<Mesh> mesh;

	template <class T>
	void addComponent(T const & c);

	//template <class T>
	//void addComponent(T c);


public:
	SharedPointer<Transform> transform;

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	template <class T>
	T & getComponent();

	virtual void loadToShader();

	GameObject(Component * component);
};