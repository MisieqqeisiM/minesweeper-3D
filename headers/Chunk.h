#ifndef CHUNK_H
#define CHUNK_H
#include<GL/glew.h>
#include<glm/glm.hpp>
#include"BasicShader.h"
class MapController;
class Chunk{
    friend class MapController;
    public:
        Chunk(int size, int x, int y, MapController * mapController);
        Chunk(int size, const char * worldName, int x, int y, MapController * mapController);
        ~Chunk();
        void draw(BasicShader& shader);
        void openCell(int x, int y);
        void flagCell(int x, int y);
        void unflagCell(int x, int y);
        void save();
        void refresh();
        float allowedMovementX (glm::vec2 pos, float requestedMovementX);
        float allowedMovementY (glm::vec2 pos, float requestedMovementY);
        static const char NOT_GENERATED = (char)0,
                          FLAGGED = (char)1,
                          MINE = (char)2,
                          EMPTY = (char)3,
                          FIELD_1 = (char)4,
                          FIELD_2 = (char)5,
                          FIELD_3 = (char)6,
                          FIELD_4 = (char)7,
                          FIELD_5 = (char)8,
                          FIELD_6 = (char)9,
                          FIELD_7 = (char)10,
                          FIELD_8 = (char)11,
                          GENERATED = (char)12;
        static const GLfloat CELL_VERTICES[32];
        static const GLfloat UNCOVERED_CELL_VERTICES[64];
        static const GLfloat FLAG_VERTICES[64];
        static const GLuint CELL_INDICES[6];
        static const GLuint UNCOVERED_CELL_INDICES[30];
        static const GLuint FLAG_INDICES[30];
        static const int CELL_VERTICES_LENGTH =  32, 
                     CELL_INDICES_LENGTH = 6;
        static const int UNCOVERED_CELL_VERTICES_LENGTH =  64, 
                     UNCOVERED_CELL_INDICES_LENGTH = 30;
        static const int FLAG_VERTICES_LENGTH =  64, 
                     FLAG_INDICES_LENGTH = 30;
        static void unloadModels();
    private:
        const char * worldName;
        int x, y;
        MapController * mapController;
        int size;
        char ** map;
        bool ** uncovered;
        bool ** flagged;
        GLuint vao, vbo, ebo;
        static GLuint uncovered_cell_vao, uncovered_cell_vbo, uncovered_cell_ebo;
        static GLuint flag_vao, flag_vbo, flag_ebo;
        int openCellCount;
        void generateVBO(GLfloat * data, int dataLength);
        void generateEBO(GLuint * data, int dataLength);
        void generateVAO();
        bool isOpen(int x, int y);
        bool isFlagged(int x, int y);
        bool isGenerated(int x, int y);
        bool isMine(int x, int y);
        void generateCell(int x, int y);
        void generateCellEmpty(int x, int y);    
        void addNewCellToData(GLfloat * vertices, GLuint * indices, int location, int index, int x, int y);
        void addCellToVBOandEBO(int x, int y, int index);
        int countOpenCells();
        char * toBytes();
        void fromBytes(char * data);
        static bool loadedModels;
        static void loadModels();
        static void enableVertexAttribArrays();
        static void loadModel(const GLfloat * vertices, int verticesLength, const GLuint * indices, int indicesLength, GLuint* vao, GLuint* vbo, GLuint* ebo);
        bool justLoaded = true;
};
#endif
