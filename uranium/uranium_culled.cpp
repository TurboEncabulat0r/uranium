#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

/*

loop through every chunk that needs to be rendered
loop through chunk and get the layer 2d array
loop through layer
mesh triangles
batch triangles

*/


#define BLOCK_SIZE 1
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 50


namespace uranium {

	// forward decl
	struct Block;
	struct Chunk;


	static Chunk* chunk;


	// used to define a block in world space
	struct Block {
		uint16_t id;
	};

	// used to define a chunk in world space
	struct Chunk {
		Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	};

	struct DrawData {

	};

	void SetBlock(Chunk& chunk, const int &x, const int &y, const int &z, const Block &block) {
		chunk.blocks[x][y][z] = block;
	}

	Block GetBlock(const Chunk& chunk, const int& x, const int& y, const int& z) {
		return chunk.blocks[x][y][z];
	}

	bool isBlockEmpty(const Block& block) {
		return block.id == 0;
	}

	bool isBlockEmpty(const Chunk& chunk, const int& x, const int& y, const int& z) {
		return isBlockEmpty(GetBlock(chunk, x, y, z));
	}

	




}