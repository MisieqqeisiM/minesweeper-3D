#include "MapController.h"
#include "Chunk.h"
#include <iostream>
MapController::MapController(){
    midX = 0;
    midY = 0;
    chunks = new Chunk**[MAP_SIZE];
    for(int i = 0; i<MAP_SIZE;i++){
        chunks[i] = new Chunk*[MAP_SIZE];
        for(int j = 0; j<MAP_SIZE; j++){
            chunks[i][j] = new Chunk(CHUNK_SIZE,"world", i-MAP_SIZE/2, j-MAP_SIZE/2, this);
        }
    }
    refreshChunks();
    int r = 3;
    for(int i = -r; i<r;i++){
        for(int j = -r; j<r;j++){
            if(i*i+j*j<=r*r)
            generateCellEmpty(i,j);
        }
    }
    openCell(0,0);
}
long long int MapController::getMidX(){
    return midX;
};
long long int MapController::getMidY(){
    return midY;
};
void MapController::saveChunks(){
    for(int i = 0; i<MAP_SIZE; i++){
        for(int j = 0; j<MAP_SIZE; j++){
            chunks[i][j]->save();
        }
    }
};
void MapController::refreshChunks(){
    for(int i = 0; i<RENDER_DISTANCE; i++){
        for(int j = 0; j<RENDER_DISTANCE; j++){
            chunks[MAP_SIZE/2-RENDER_DISTANCE/2+i][MAP_SIZE/2-RENDER_DISTANCE/2+j]->refresh();
        }
    }
};
MapController::~MapController(){
    for(int i = 0; i<MAP_SIZE; i++){
        for(int j = 0; j<MAP_SIZE; j++){
            delete chunks[i][j];
        }
        delete[] chunks[i];
    }
    delete[] chunks;
};

void MapController::draw(BasicShader& shader){
    for(int i = -RENDER_DISTANCE/2; i<=RENDER_DISTANCE/2; i++){
        for(int j = -RENDER_DISTANCE/2; j<=RENDER_DISTANCE/2; j++){
            chunks[i+MAP_SIZE/2][j+MAP_SIZE/2] -> draw(shader);
        }
    }
};
void MapController::openCell(int x, int y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    if(chunkX>=-MAP_SIZE/2&&chunkY>=-MAP_SIZE/2&&chunkX<=MAP_SIZE/2&&chunkY<=MAP_SIZE/2)
        chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2]->openCell(x-chunkX*CHUNK_SIZE, y-chunkY*CHUNK_SIZE);
}
void MapController::flagCell(int x, int y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    if(chunkX>=-MAP_SIZE/2&&chunkY>=-MAP_SIZE/2&&chunkX<=MAP_SIZE/2&&chunkY<=MAP_SIZE/2)
        chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2]->flagCell(x-chunkX*CHUNK_SIZE, y-chunkY*CHUNK_SIZE);
}
void MapController::unflagCell(int x, int y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    if(chunkX>=-MAP_SIZE/2&&chunkY>=-MAP_SIZE/2&&chunkX<=MAP_SIZE/2&&chunkY<=MAP_SIZE/2)
        chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2]->unflagCell(x-chunkX*CHUNK_SIZE, y-chunkY*CHUNK_SIZE);
}
bool MapController::isMine(int x, int y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    if(chunkX>=-MAP_SIZE/2&&chunkY>=-MAP_SIZE/2&&chunkX<=MAP_SIZE/2&&chunkY<=MAP_SIZE/2)
        return chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2]->isMine(x-chunkX*CHUNK_SIZE, y-chunkY*CHUNK_SIZE);
    else
        return false;
}

void MapController::generateCellEmpty(int x, int y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    if(chunkX>=-MAP_SIZE/2&&chunkY>=-MAP_SIZE/2&&chunkX<=MAP_SIZE/2&&chunkY<=MAP_SIZE/2)
        chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2]->generateCellEmpty(x-chunkX*CHUNK_SIZE, y-chunkY*CHUNK_SIZE);
}

int MapController::countMinesAround(int x, int y){
    int count = 0;
    for(int i = -1; i<=1;i++){
        for(int j = -1; j<=1; j++){
             if(isMine(x+i, y+j))
                 count++;
        }
    }
    return count;
}
void MapController::openCellsAround(int x, int y){
    for(int i = -1; i<=1;i++)
        for(int j = -1; j<=1; j++){
            openCell(x+i, y+j);
        }
}
void MapController::shiftRight(){
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[MAP_SIZE-1][i]->save();
        delete chunks[MAP_SIZE-1][i];
    }
    for(int i = MAP_SIZE-1; i>0;i--){
        for(int j = 0; j<MAP_SIZE;j++){
            chunks[i][j] = chunks[i-1][j];
            chunks[i][j]->x += 1;
        }
    }
    midX--;
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[0][i] = new Chunk(CHUNK_SIZE,"world",-MAP_SIZE/2, i-MAP_SIZE/2, this);
    }
    refreshChunks();
}
void MapController::shiftLeft(){
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[0][i]->save();
        delete chunks[0][i];
    }
    for(int i = 0; i<MAP_SIZE-1;i++){
        for(int j = 0; j<MAP_SIZE;j++){
            chunks[i][j] = chunks[i+1][j];
            chunks[i][j]->x -= 1;
        }
    }
    midX++;
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[MAP_SIZE-1][i] = new Chunk(CHUNK_SIZE,"world", MAP_SIZE/2, i-MAP_SIZE/2, this);
    }
    refreshChunks();
}
void MapController::shiftUp(){
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[i][MAP_SIZE-1]->save();
        delete chunks[i][MAP_SIZE-1];
    }
    for(int i = MAP_SIZE-1; i>0;i--){
        for(int j = 0; j<MAP_SIZE;j++){
            chunks[j][i] = chunks[j][i-1];
            chunks[j][i] ->y += 1;
        }
    }
    midY--;
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[i][0] = new Chunk(CHUNK_SIZE, "world", i-MAP_SIZE/2, -MAP_SIZE/2, this);
    }
    refreshChunks();
}
void MapController::shiftDown(){
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[i][0]->save();
        delete chunks[i][0];
    }
    for(int i = 0; i<MAP_SIZE-1;i++){
        for(int j = 0; j<MAP_SIZE;j++){
            chunks[j][i] = chunks[j][i+1];
            chunks[j][i] ->y -= 1;
        }
    }
    midY++;
    for(int i = 0; i<MAP_SIZE; i++){
        chunks[i][MAP_SIZE-1] = new Chunk(CHUNK_SIZE, "world", i-MAP_SIZE/2, MAP_SIZE/2, this);
    }
    refreshChunks();
}

Chunk * MapController::getChunk(float x, float y){
    int chunkX = std::floor((float)x/CHUNK_SIZE);
    int chunkY = std::floor((float)y/CHUNK_SIZE);
    return chunks[chunkX+MAP_SIZE/2][chunkY+MAP_SIZE/2];
};
glm::vec2 MapController::allowedMovement (glm::vec2 pos, glm::vec2 requestedMovement){
   return glm::vec2(getChunk(pos.x+requestedMovement.x, pos.y)->allowedMovementX(pos, requestedMovement.x), getChunk(pos.x, pos.y+requestedMovement.y)->allowedMovementY(pos, requestedMovement.y)); 
};


