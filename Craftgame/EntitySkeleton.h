#pragma once
#include "AdvancedModel.h"
#include <map>
#include "Entity.h"

class EntitySkeleton :
	public Entity
{
protected:
	glm::mat4 armTransform;
public:
	typedef std::map<std::string, SkeletalAnimation> mtype;
	AdvancedAssimpModel* model;
	/*
	��������� ��������.
		name - ��������
		initial - 0 - �����, 1 - ������������
		repeating - ���������� ��������
	
	applyAnimation, � ������� �� updateAnimation, ����� ������ ����� ��������
	*/
	SkeletalAnimation& applyAnimation(std::string name, char initial = 1, bool repeating = true);
	SkeletalAnimation& updateAnimation(std::string name, char initial = 1, bool repeating = true);
	EntitySkeleton() :
		model(nullptr) {}
	EntitySkeleton(World* world, const size_t& _id);
	virtual ~EntitySkeleton();
	virtual void render();
	virtual glm::mat4 getArmTransform();
protected:
	mtype mAnimations;
};

