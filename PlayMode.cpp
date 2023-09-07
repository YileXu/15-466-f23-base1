#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

//https://www.geeksforgeeks.org/how-to-return-multiple-values-from-a-function-in-c-or-cpp/
struct ppuAndRCNum {
    PPU466 ppu;
	std::vector<int> rowAndColNum;
};

ppuAndRCNum spacemanStruct;
std::vector<int> spacemanRowAndColNum = {0, 0};
std::string spacemanFilename = "./Assets/Sprites/Spaceman.png";
glm::uvec2 spacemanSize;
std::vector< glm::u8vec4 > spacemanData;
OriginLocation spacemanOrigin = LowerLeftOrigin;
std::vector<int> spacemanIndexAndAttributes = {100, 1};

ppuAndRCNum fireStruct;
std::vector<int> fireRowAndColNum = {0, 0};
std::string fireFilename = "./Assets/Sprites/Fire.png";
glm::uvec2 fireSize;
std::vector< glm::u8vec4 > fireData;
OriginLocation fireOrigin = LowerLeftOrigin;
std::vector<int> fireIndexAndAttributes = {220, 2};

ppuAndRCNum fireLStruct;
std::vector<int> fireLRowAndColNum = {0, 0};
std::string fireLFilename = "./Assets/Sprites/FireL.png";
glm::uvec2 fireLSize;
std::vector< glm::u8vec4 > fireLData;
OriginLocation fireLOrigin = LowerLeftOrigin;
std::vector<int> fireLIndexAndAttributes = {230, 3};

ppuAndRCNum fireRStruct;
std::vector<int> fireRRowAndColNum = {0, 0};
std::string fireRFilename = "./Assets/Sprites/FireR.png";
glm::uvec2 fireRSize;
std::vector< glm::u8vec4 > fireRData;
OriginLocation fireROrigin = LowerLeftOrigin;
std::vector<int> fireRIndexAndAttributes = {240, 4};

ppuAndRCNum load_png_stronger(std::string filename, glm::uvec2 size, std::vector< glm::u8vec4 > data, OriginLocation origin, std::vector<int> rowAndColNum, PPU466 ppu, int spriteIndexStart, int paletteTableIndex) {

	load_png(filename, &size, &data, origin);

	// create paletteTable
	int paletteTableSize = 0;
	for (int i=0; i<data.size(); i++) {
		bool paletteTableMatched = false;
		for (int j=0; j<paletteTableSize; j++) {
			if (data[i] == ppu.palette_table[paletteTableIndex][j]) {
				paletteTableMatched = true;
			}
		}
		if (!paletteTableMatched) {
			ppu.palette_table[paletteTableIndex][paletteTableSize] = (data[i]);
			paletteTableSize++;
		}
	}

	// create player vector<int> in 0123
	std::vector<int> player;
	for (int i=0; i<data.size(); i++) {
		for (int j=0; j<4; j++) {
			if (data[i] == ppu.palette_table[paletteTableIndex][j]) {
				player.push_back(j);
			}
		}
	}

	// store player vector<int> in bit0's 01 and bit1's 01
	std::vector<int> bit0Data(player.size());
	std::vector<int> bit1Data(player.size());
	for (int k=0; k<player.size(); k++) {
		bit0Data[k] = player[k]%2;
		bit1Data[k] = player[k]/2;
	}

	// create bit0s and bit1s
	rowAndColNum[0] = size[0]/8;
	rowAndColNum[1] = size[1]/8;

	for (int i=0; i<rowAndColNum[0]; i++) {
		for (int j=0; j<rowAndColNum[1]; j++) {
			ppu.tile_table[spriteIndexStart+j*rowAndColNum[0]+i].bit0 = {};
			ppu.tile_table[spriteIndexStart+j*rowAndColNum[0]+i].bit1 = {};
			for (int k=0; k<8; k++) {
				// can be used in vector<char> but not vector<int> ;(
				// std::string bit0Str(bit0Data.begin()+(i+k)*size[0]+j*8, bit0Data.begin()+(i+k)*size[0]+j*8+8-1);
				// std::string bit1Str(bit1Data.begin()+(i+k)*size[0]+j*8, bit1Data.begin()+(i+k)*size[0]+j*8+8-1);
				int bit0Accum = 0;
				int bit1Accum = 0;
				for (int m=0; m<8; m++) {
					bit0Accum += bit0Data[(j*8+k)*size[0]+i*8+m]*pow(2, m);
					bit1Accum += bit1Data[(j*8+k)*size[0]+i*8+m]*pow(2, m);
				}
				ppu.tile_table[spriteIndexStart+j*rowAndColNum[0]+i].bit0[k] = bit0Accum;
				ppu.tile_table[spriteIndexStart+j*rowAndColNum[0]+i].bit1[k] = bit1Accum;
			}
		}
	}

	struct ppuAndRCNum ans;
	ans.ppu = ppu;
	ans.rowAndColNum = rowAndColNum;
	return ans;
};

PlayMode::PlayMode() {
	// TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	// make yourself a script that spits out the code that you paste in here
	// and check that script into your repository.
	
	//bg paletteTable
	ppu.palette_table[0] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
	};

	spacemanStruct = load_png_stronger(spacemanFilename, spacemanSize, spacemanData, spacemanOrigin, spacemanRowAndColNum, ppu, spacemanIndexAndAttributes[0], spacemanIndexAndAttributes[1]);
	ppu = spacemanStruct.ppu;
	spacemanRowAndColNum = spacemanStruct.rowAndColNum;

	fireStruct = load_png_stronger(fireFilename, fireSize, fireData, fireOrigin, fireRowAndColNum, ppu, fireIndexAndAttributes[0], fireIndexAndAttributes[1]);
	ppu = fireStruct.ppu;
	fireRowAndColNum = fireStruct.rowAndColNum;

	fireLStruct = load_png_stronger(fireLFilename, fireLSize, fireLData, fireLOrigin, fireLRowAndColNum, ppu, fireLIndexAndAttributes[0], fireLIndexAndAttributes[1]);
	ppu = fireLStruct.ppu;
	fireLRowAndColNum = fireLStruct.rowAndColNum;

	fireRStruct = load_png_stronger(fireRFilename, fireRSize, fireRData, fireROrigin, fireRRowAndColNum, ppu, fireRIndexAndAttributes[0], fireRIndexAndAttributes[1]);
	ppu = fireRStruct.ppu;
	fireRRowAndColNum = fireRStruct.rowAndColNum;
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	// if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	player_at.y -= PlayerSpeed * elapsed / 2;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	ppu.background_color = glm::u8vec4(0x00, 0x00, 0x22, 0xff);
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			ppu.background[x+PPU466::BackgroundHeight*y] = (0);
		}
	}

	int curSpriteIndex;

	//player sprite:
	int playerSpriteStart = 0;
	for (int i=0; i<spacemanRowAndColNum[0]*spacemanRowAndColNum[1]; i++) {
		ppu.sprites[playerSpriteStart+i].x = int8_t(player_at.x+PPU466::ScreenWidth/2-spacemanRowAndColNum[0]*8/2+(i%spacemanRowAndColNum[0])*8);
		ppu.sprites[playerSpriteStart+i].y = int8_t(player_at.y+PPU466::ScreenHeight/2-spacemanRowAndColNum[1]*8/2+(i/spacemanRowAndColNum[0])*8);
		ppu.sprites[playerSpriteStart+i].index = spacemanIndexAndAttributes[0]+i;
		ppu.sprites[playerSpriteStart+i].attributes = spacemanIndexAndAttributes[1];
		curSpriteIndex = playerSpriteStart+i;
	}

	std::vector<int> fireInitPos = {-5, -36};
	int fireSpriteStart = curSpriteIndex+1;
	for (int i=0; i<fireRowAndColNum[0]*fireRowAndColNum[1]; i++) {
		ppu.sprites[fireSpriteStart+i].x = int8_t(fireInitPos[0] + player_at.x+PPU466::ScreenWidth/2-fireRowAndColNum[0]*8/2+(i%fireRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].y = int8_t(fireInitPos[1] + player_at.y+PPU466::ScreenHeight/2-fireRowAndColNum[1]*8/2+(i/fireRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].index = fireIndexAndAttributes[0]+i;
		if (up.pressed) {ppu.sprites[fireSpriteStart+i].attributes = fireIndexAndAttributes[1];} else {ppu.sprites[fireSpriteStart+i].attributes = 0;}
		curSpriteIndex = fireSpriteStart+i;
	}

	fireInitPos = {5, -36};
	fireSpriteStart = curSpriteIndex+1;
	for (int i=0; i<fireRowAndColNum[0]*fireRowAndColNum[1]; i++) {
		ppu.sprites[fireSpriteStart+i].x = int8_t(fireInitPos[0] + player_at.x+PPU466::ScreenWidth/2-fireRowAndColNum[0]*8/2+(i%fireRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].y = int8_t(fireInitPos[1] + player_at.y+PPU466::ScreenHeight/2-fireRowAndColNum[1]*8/2+(i/fireRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].index = fireIndexAndAttributes[0]+i;
		if (up.pressed) {ppu.sprites[fireSpriteStart+i].attributes = fireIndexAndAttributes[1];} else {ppu.sprites[fireSpriteStart+i].attributes = 0;}
		curSpriteIndex = fireSpriteStart+i;
	}

	std::vector<int> fireLInitPos = {-20, 3};
	fireSpriteStart = curSpriteIndex+1;
	for (int i=0; i<fireLRowAndColNum[0]*fireLRowAndColNum[1]; i++) {
		ppu.sprites[fireSpriteStart+i].x = int8_t(fireLInitPos[0] + player_at.x+PPU466::ScreenWidth/2-fireLRowAndColNum[0]*8/2+(i%fireLRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].y = int8_t(fireLInitPos[1] + player_at.y+PPU466::ScreenHeight/2-fireLRowAndColNum[1]*8/2+(i/fireLRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].index = fireLIndexAndAttributes[0]+i;
		if (right.pressed) {ppu.sprites[fireSpriteStart+i].attributes = fireLIndexAndAttributes[1];} else {ppu.sprites[fireSpriteStart+i].attributes = 0;}
		curSpriteIndex = fireSpriteStart+i;
	}

	std::vector<int> fireRInitPos = {20, 3};
	fireSpriteStart = curSpriteIndex+1;
	for (int i=0; i<fireRRowAndColNum[0]*fireRRowAndColNum[1]; i++) {
		ppu.sprites[fireSpriteStart+i].x = int8_t(fireRInitPos[0] + player_at.x+PPU466::ScreenWidth/2-fireRRowAndColNum[0]*8/2+(i%fireRRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].y = int8_t(fireRInitPos[1] + player_at.y+PPU466::ScreenHeight/2-fireRRowAndColNum[1]*8/2+(i/fireRRowAndColNum[0])*8);
		ppu.sprites[fireSpriteStart+i].index = fireRIndexAndAttributes[0]+i;
		if (left.pressed) {ppu.sprites[fireSpriteStart+i].attributes = fireRIndexAndAttributes[1];} else {ppu.sprites[fireSpriteStart+i].attributes = 0;}
		curSpriteIndex = fireSpriteStart+i;
	}

	//--- actually draw ---
	ppu.draw(drawable_size);
}
