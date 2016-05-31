#pragma once

#include "Commons.h"
#include "TransformEntity.h"
#include "Shader.h"
#include "Material.h"

/* Components are like Graphical Resources that one loads from files */
class Component : BaseEntity {
private:
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

protected:
	Material * material;

public:
	Shader * attachedShader;


	//virtual void render();
	//virtual void destroy();

	Component& setMaterial(Material * mat);
	Component& attachShader(Shader * shader);

	Shader& getShader();
	Material& getMaterial();

	~Component();


	static SharedPointer<Component> EMPTY;
};