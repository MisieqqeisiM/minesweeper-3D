#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H
#include "BasicShader.h"
class Chunk;
class MapController{
    public:
        static const int CHUNK_SIZE =  32;
        static const int MAP_SIZE = 7;
        static const int RENDER_DISTANCE = 5;
        
        MapController();
        ~MapController();
        void draw(BasicShader& shader);
        void openCell(int x, int y);
        void flagCell(int x, int y);
        void unflagCell(int x, int y);
        bool isOpen(int x, int y);
        bool isFlagged(int x, int y);
        bool isGenerated(int x, int y);
        bool isMine(int x, int y);
        void saveChunks();
        int countMinesAround(int x, int y);
        void openCellsAround(int x, int y);
        void shiftLeft();
        void shiftRight();
        void shiftUp();
        void shiftDown();
        long long int getMidX();
        long long int getMidY();
        glm::vec2 allowedMovement (glm::vec2 pos, glm::vec2 requestedMovement);
    private:

        void generateCellEmpty(int x, int y);
        Chunk *** chunks;
        Chunk * getChunk(float x, float y);
        long long int midX;
        long long int midY;
        void refreshChunks();

};
#endif
