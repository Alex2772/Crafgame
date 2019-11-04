#pragma once
#include <list>
#include <deque>
#include "IChunkTracker.h"
#include "ts.h"
#include "def.h"
#include "gl.h"
#include "ChunkMesher.h"
#include <atomic>
#include "ThreadUtils.h"
#include "Material.h"
#include "global.h"
#include "AABB.h"
#include "Block.h"

class Entity;
class World;
class TileEntity;
class Block;
struct BlockFaces
{
	bool left, right, front, back, top, bottom;
};

class ChunkPart
{
public:

	GLuint vao = 0;
	/**
	* \brief ������.
	* 0 - �������
	* 1 - ���������� ����������
	* 2 - �������
	* 3 - �������
	* 4 - �������
	*/
	GLuint buffers[5] = { 0 };
	ChunkMesher* mesher = new ChunkMesher;
	Material* material;
	ChunkPart() {}
	ChunkPart(const ChunkPart&) = delete;
	~ChunkPart();
	void destroyMesher()
	{
		delete mesher;
		mesher = nullptr;
	}
	std::atomic<GLsizei> icount;
};
class ChunkPartHandler
{
private:
	std::vector<ChunkPart*> _infos;
public:

	ChunkPartHandler();
	~ChunkPartHandler();
	bool isEmpty() const;

	inline std::vector<ChunkPart*>::iterator begin()
	{
		return _infos.begin();
	}
	inline std::vector<ChunkPart*>::iterator end()
	{
		return _infos.end();
	}
	ChunkPart* getCI(Material* m);
};

class Chunk
{
private:
	class CPos
	{
	private:
		int toInt() const;
	public:
		int8_t x;
		int8_t y;
		int8_t z;
		CPos();
		CPos(int8_t x, int8_t y, int8_t z);
		bool operator>(const CPos& b) const;
		bool operator<(const CPos& b) const;
	};

	/**
	 * \brief ���������� ��������� ������
	 * ######## ######## ######## ########
	 * |           ID            | DATA  | 
	 */
	uint32_t data[16][16][16];
	/**
	 * \brief TileEntity
	 */
	std::map<CPos, TileEntity*> tileEntities;

	Pos pos;
	World* worldObj;
	
#ifndef SERVER
	/**
	* \brief ���������
	*/
	enum State
	{
		UNKNOWN = 0,
		RENDER = 1, // ����� ���������
		DIRTY = 2, // ��������� �����
	};
	std::atomic<char> state = UNKNOWN;

	/**
	* \brief ����� - �����, ���������� �� ���������� ������ � 1 ���. ������������� ������� ���������� ��������� � ��������� ��������� ������� � ���.
	*/
	ChunkPartHandler* mesher;

	/**
	 * \brief ����� � ���������� �����������
	 */
	std::deque<TileEntity*> teCR;
#endif

	static uint32_t toChunkFormat(Block* b);
	static Block* fromChunkFormat(uint32_t cf);

	Chunk* cLeft;
	Chunk* cRight;
	Chunk* cBack;
	Chunk* cFront;
	Chunk* cTop;
	Chunk* cBottom;
public:
	std::list<Entity*> entityList;

	/**
	 * \brief �������, "��������" �� ���� ������.
	 */
	std::deque<IChunkTracker*> mChunkTrackers;

	bool modified = false;

	Chunk(World* w, Pos p);
	~Chunk();
	
	TileEntity* getTileEntity(Pos p);
	Block* getBlock(Pos p);
	bool setBlock(Block* block, Pos p);
	bool setTileEntity(TileEntity* e, Pos p);
	Pos& getPos()
	{
		return pos;
	}

	void unload();
	void remeshAsync();
	void markAsDirty();
	bool blockCheck(Pos p);
	void remesh();
	void render(byte);
	void flush(ChunkPartHandler* mesher);
	void remeshAround();
	void save();
	void load();
	bool isVisible();

	Chunk* getCLeft();
	Chunk* getCRight();
	Chunk* getCBack();
	Chunk* getCFront();
	Chunk* getCTop();
	Chunk* getCBottom();
	AABB createAABB();
	void clear();
};

