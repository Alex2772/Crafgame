#pragma once


class InstancerBase
{
public:
	virtual ~InstancerBase() = default;
	virtual char* create() = 0;
};

/**
 * \brief ��������� ������� ��������� ����������� ����
 * \tparam T 
 */
template<typename T>
class Instancer: public InstancerBase
{
public:
	virtual ~Instancer() = default;

	virtual char* create()
	{
		return reinterpret_cast<char*>(new T);
	}
};