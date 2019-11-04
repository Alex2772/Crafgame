#pragma once
#include "AdvancedModel.h"

class Animatronic
{
public:
	typedef std::map<std::string, SkeletalAnimation> mtype;
	mtype mAnimations;

	/*
	��������� ��������.
	name - ��������
	initial - 0 - �����, 1 - ������������
	repeating - ���������� ��������

	applyAnimation, � ������� �� updateAnimation, ����� ������ ����� ��������
	*/
	SkeletalAnimation& applyAnimation(std::string name, char initial = 1, bool repeating = true);
	SkeletalAnimation& updateAnimation(std::string name, char initial = 1, bool repeating = true);
};
