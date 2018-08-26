#include "Chunk.h"
#include "MapController.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>


const GLfloat Chunk::UNCOVERED_CELL_VERTICES[64] = {
    0.0f,0.0f,1.0f, 11.01f,11.01f, -0.707f,0.707f,0.0f,
    1.0f,0.0f,1.0f, 11.99f,11.01f, 0.0f,0.707f,0.707f,
    1.0f,0.0f,0.0f, 11.99f,11.99f, 0.707f,0.707f,0.0f,
    0.0f,0.0f,0.0f, 11.01f,11.99f, 0.0f,0.707f,-0.707f,

    0.2f,0.2f,0.8f, 11.2f,11.2f, 0.0f,1.0f,0.0f,
    0.8f,0.2f,0.8f, 11.8f,11.2f, 0.0f,1.0f,0.0f,
    0.8f,0.2f,0.2f, 11.8f,11.8f, 0.0f,1.0f,0.0f,
    0.2f,0.2f,0.2f, 11.2f,11.8f, 0.0f,1.0f,0.0f
};
const GLfloat Chunk::FLAG_VERTICES[64] = {
    0.49f,0.2f,0.51f, 12.5f,0.5f, -1.0f,0.0f,0.0f,
    0.51f,0.2f,0.51f, 12.5f,0.5f, 0.0f,0.0f,1.0f,
    0.51f,0.2f,0.49f, 12.5f,0.5f, 1.0f,0.0f,0.0f,
    0.49f,0.2f,0.49f, 12.5f,0.5f, 0.0f,0.0f,-1.0f,

    0.49f,1.5f,0.51f, 12.5f,0.5f, 0.0f,1.0f,0.0f,
    0.51f,1.5f,0.51f, 12.5f,0.5f, 0.0f,1.0f,0.0f,
    0.51f,1.5f,0.49f, 12.5f,0.5f, 0.0f,1.0f,0.0f,
    0.49f,1.5f,0.49f, 12.5f,0.5f, 0.0f,1.0f,0.0f
};
const GLuint Chunk::FLAG_INDICES[30] = {
    0,7,3,
    0,4,7,
    1,4,0,
    1,5,4,
    2,5,1,
    2,6,5,
    3,6,2,
    3,7,6,
    4,5,6,
    4,6,7
};

const GLfloat Chunk::CELL_VERTICES[32] = {
    0.0f,0.0f,1.0f, 0.01f,0.01f, 0.0f,1.0f,0.0f,
    1.0f,0.0f,1.0f, 0.99f,0.01f, 0.0f,1.0f,0.0f,
    1.0f,0.0f,0.0f, 0.99f,0.99f, 0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f, 0.01f,0.99f, 0.0f,1.0f,0.0f
};
const GLuint Chunk::UNCOVERED_CELL_INDICES[30] = {
    0,7,3,
    0,4,7,
    1,4,0,
    1,5,4,
    2,5,1,
    2,6,5,
    3,6,2,
    3,7,6,
    4,5,6,
    4,6,7
};
const GLuint Chunk::CELL_INDICES[6] = {
    0,1,2,
    3,0,2
};
GLuint Chunk::uncovered_cell_ebo = 0;
GLuint Chunk::uncovered_cell_vbo = 0;
GLuint Chunk::uncovered_cell_vao = 0;

GLuint Chunk::flag_ebo = 0;
GLuint Chunk::flag_vbo = 0;
GLuint Chunk::flag_vao = 0;
bool Chunk::loadedModels = false;
void Chunk::loadModels(){
    if(!loadedModels){
        loadedModels = true;
        loadModel(UNCOVERED_CELL_VERTICES, UNCOVERED_CELL_VERTICES_LENGTH, UNCOVERED_CELL_INDICES, UNCOVERED_CELL_INDICES_LENGTH, &uncovered_cell_vao, &uncovered_cell_vbo, &uncovered_cell_ebo);
        loadModel(FLAG_VERTICES, FLAG_VERTICES_LENGTH, FLAG_INDICES, FLAG_INDICES_LENGTH, &flag_vao, &flag_vbo, &flag_ebo);
    }
}

void Chunk::loadModel(const GLfloat * vertices, int verticesLength, const GLuint * indices, int indicesLength, GLuint* vao, GLuint* vbo, GLuint* ebo){
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesLength*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,*ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength*sizeof(GLuint), indices, GL_STATIC_DRAW);
    enableVertexAttribArrays();
}; 
Chunk::Chunk(int size, int x, int y, MapController * mapController){
    loadModels();
    this->mapController = mapController;
    this->x = x;
    this->y = y;
    this->size = size;
    this->map = new char*[size];
    this->flagged = new bool*[size];
    this->uncovered = new bool*[size];
    for(int i = 0; i < size; i++){
        this->map[i] = new char[size];
        this->uncovered[i] = new bool[size]; 
        this->flagged[i] = new bool[size];
        for(int j = 0; j<size; j++){
            generateCell(i,j);
            this->uncovered[i][j] = false;
            this->flagged[i][j] = false;
        }
    }
    generateVAO();
    generateVBO(nullptr, 0);
    generateEBO(nullptr, 0);
    glBindVertexArray(0);
    openCellCount = 0;
};

bool Chunk::isGenerated(int x, int y){
    return map[x][y] != NOT_GENERATED;
}

Chunk::Chunk(int size, const char* worldName, int x, int y, MapController * mapController){
    loadModels();
    this->x = x;
    this->y = y;
    this->worldName = worldName;
    this->map = new char*[size];
    this->flagged = new bool*[size];
    this->uncovered = new bool*[size];
    this->size = size;
    this->mapController = mapController;
    for(int i = 0; i < size; i++){
        this->map[i] = new char[size];
        this->uncovered[i] = new bool[size]; 
        this->flagged[i] = new bool[size];
        for(int j = 0; j<size; j++){
            this->map[i][j] = NOT_GENERATED;
            this->uncovered[i][j] = false;
            this->flagged[i][j] = false;
        }
    }
    std::string fileName = "./"+std::string(worldName) +"/"+std::to_string(mapController->getMidX()+this->x) +"_"+std::to_string(mapController->getMidY()+this->y)+".chunk";
    std::ifstream f(fileName.c_str(), std::ios::binary);
    if(f.good()){
        int dataLength = 3*(int)std::ceil((float)(size*size)/8.0f);
        char * data = new char[dataLength];
        f.read(data, dataLength);
        f.close();
        fromBytes(data);
    }else{
        for(int i = 0; i < size; i++){
            for(int j = 0; j<size; j++){
                generateCell(i,j);
            }
        }
        generateVAO();
        generateVBO(nullptr, 0);
        generateEBO(nullptr, 0);
        glBindVertexArray(0);
        justLoaded = false;
        openCellCount = 0;
    }

}

void Chunk::refresh(){
    if(justLoaded){
        justLoaded = false;
        openCellCount = countOpenCells();
        GLfloat * vertices = new GLfloat[openCellCount*CELL_VERTICES_LENGTH];
        GLuint * indices = new GLuint[openCellCount*CELL_INDICES_LENGTH];
        int count = 0;
        for(int i = 0; i<size; i++){
            for(int j = 0; j<size; j++){
                if(isOpen(i,j)){
                    if(!isMine(i,j))
                        map[i][j] = mapController->countMinesAround(MapController::CHUNK_SIZE*this->x+i, MapController::CHUNK_SIZE*this->y+j)+EMPTY;

                    addNewCellToData(vertices, indices, count, count, i, j);   
                    count++;
                }
            }
        }
        generateVAO();
        generateVBO(vertices, openCellCount*CELL_VERTICES_LENGTH*sizeof(GLfloat));
        generateEBO(indices, openCellCount*CELL_INDICES_LENGTH*sizeof(GLuint));  
        glBindVertexArray(0);
        delete[]vertices;
        delete[]indices;
    }
};
int Chunk::countOpenCells(){
    int openCellCount = 0;
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(isOpen(i,j))
                openCellCount++;
        }
    }
    return openCellCount;
}; 
void Chunk::openCell(int x, int y){
    if(x>=0&&y>=0&&x<size&&y<size){
        if(!isOpen(x, y)&&!isFlagged(x, y)){
            if(!isMine(x,y))
                map[x][y] = mapController->countMinesAround(MapController::CHUNK_SIZE*this->x+x, MapController::CHUNK_SIZE*this->y+y)+EMPTY;

            uncovered[x][y] = true;
            addCellToVBOandEBO(x, y, openCellCount);
            openCellCount++;
            if(map[x][y] == EMPTY){
                mapController->openCellsAround(MapController::CHUNK_SIZE*this->x+x, MapController::CHUNK_SIZE*this->y+y);
            }
        }
    }
}

void Chunk::flagCell(int x, int y){
    if(!isOpen(x, y))
        flagged[x][y] = true;
}
void Chunk::unflagCell(int x, int y){
    if(isFlagged(x,y))
        flagged[x][y] = false;
}
void Chunk::generateCell(int x, int y){
    if(!isGenerated(x,y)){
        if(rand()%100>80)
            map[x][y] = MINE;
        else
            map[x][y] = GENERATED;
    }
}
bool Chunk::isMine(int x, int y){
    return map[x][y] == MINE;
}
float Chunk::allowedMovementX(glm::vec2 position, float requestedMovementX){
    float ans = requestedMovementX;
    int cellX = std::floor(position.x+requestedMovementX)-MapController::CHUNK_SIZE*this->x;
    int cellY = std::floor(position.y) -MapController::CHUNK_SIZE*this->y;
    if(!isOpen(cellX,cellY))
        ans = 0;
    return ans;
};
float Chunk::allowedMovementY(glm::vec2 position, float requestedMovementY){
    float ans = requestedMovementY;
    int cellX = std::floor(position.x)-MapController::CHUNK_SIZE*this->x;
    int cellY = std::floor(position.y+requestedMovementY)-MapController::CHUNK_SIZE*this->y;
    if(!isOpen(cellX,cellY))
        ans = 0;
    return ans;
};

void Chunk::addCellToVBOandEBO(int x, int y, int index){
    GLfloat * vertices = new GLfloat[CELL_VERTICES_LENGTH];
    GLuint * indices = new GLuint[CELL_INDICES_LENGTH];
    addNewCellToData(vertices, indices, 0, index, x, y);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, index*CELL_VERTICES_LENGTH*sizeof(GLfloat), CELL_VERTICES_LENGTH*sizeof(GLfloat), vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index*CELL_INDICES_LENGTH*sizeof(GLuint), CELL_INDICES_LENGTH*sizeof(GLuint), indices);
    glBindVertexArray(0);
};

void Chunk::addNewCellToData(GLfloat * vertices, GLuint * indices, int location, int index, int x, int y){
    for(int i = 0; i<CELL_VERTICES_LENGTH; i++)
        vertices[location*CELL_VERTICES_LENGTH+i] = CELL_VERTICES[i]+(i%8==0?x:0) + (i%8==2?y:0)+(i%8==3?(map[x][y]-1):0);  
    for(int i = 0; i<CELL_INDICES_LENGTH; i++)
        indices[location*CELL_INDICES_LENGTH+i] = CELL_INDICES[i]+index*CELL_VERTICES_LENGTH/8;  
} 

Chunk::~Chunk(){
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    for(int i = 0; i<size; i++){
        delete [] map[i];
        delete [] uncovered[i];
        delete [] flagged[i];
    }
    delete[] map;
    delete[] uncovered;
    delete[] flagged;
};
void Chunk::generateVAO(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}
void Chunk::generateCellEmpty(int x, int y){
    if(!isOpen(x, y)){
        map[x][y] = GENERATED;
    }
};
void Chunk::save(){
    std::string fileName = "./"+std::string(worldName) +"/"+std::to_string(mapController->getMidX()+x) +"_"+std::to_string(mapController->getMidY()+y)+".chunk";
    std::ofstream f(fileName.c_str(), std::ios::binary);
    char * data = toBytes();
    for(int i = 0; i<3*(int)std::floor((float)(size*size)/8.0f);i++)
        f<<data[i];
    f.flush();
    f.close();
    delete [] data;
}
char * Chunk::toBytes(){
    int cellCount = size*size;
    int byteCount = (int)std::ceil((float)cellCount/8.0f);
    char * ans = new char[3*byteCount];
    int currentBit = 0;
    int currentByte = 0;
    ans[0] = 0b00000000;
    ans[byteCount] = 0b00000000;
    ans[2*byteCount] = 0b00000000;
    for(int i = 0; i<size; i++){
        for(int j=0;j<size;j++){
            if(isMine(i,j))
                ans[currentByte] |= 0b10000000>>currentBit;
            if(isOpen(i,j))
                ans[byteCount+currentByte] |= 0b10000000>>currentBit;
            if(isFlagged(i,j))
                ans[2*byteCount+currentByte] |= 0b10000000>>currentBit;

            currentBit++;
            if(currentBit==8){
                currentByte++;
                if(currentByte < byteCount){
                    ans[currentByte] = 0b00000000;
                    ans[byteCount+currentByte] = 0b00000000;
                    ans[byteCount*2+currentByte] = 0b00000000;
                }
                currentBit = 0;
            }
        }
    }

    return ans;
};
void Chunk::fromBytes(char * data){
    int cellCount = size*size;
    int byteCount = (int)std::ceil((float)cellCount/8.0f);

    int currentBit = 0;
    int currentByte = 0;
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if((data[currentByte]&(0b10000000>>currentBit)))
                map[i][j] = MINE;
            else
                map[i][j] = GENERATED;
            if((data[byteCount+currentByte]&(0b10000000>>currentBit)))
                uncovered[i][j] = true;
            if((data[byteCount*2+currentByte]&(0b10000000>>currentBit)))
                flagged[i][j] = true;

            currentBit++;
            if(currentBit==8){
                currentByte++;
                currentBit=0;
            }
        }
    }
};
void Chunk::generateVBO(GLfloat * data, int dataLength){
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size*size*CELL_VERTICES_LENGTH*sizeof(GLfloat), (GLvoid*)0, GL_STATIC_DRAW);
    if(dataLength>0)
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataLength, data);
    enableVertexAttribArrays();

};
void Chunk::enableVertexAttribArrays(){
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (0*sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (3*sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (5*sizeof(GLfloat)));

};
void Chunk::generateEBO(GLuint * data, int dataLength){
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*size*CELL_INDICES_LENGTH*sizeof(GLuint), (GLvoid *)0, GL_STATIC_DRAW);
    if(dataLength>0)
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dataLength, data);
};
bool Chunk::isOpen(int x, int y){
    return x>=0&&x<size&&y>=0&&y<size&&uncovered[x][y]==true;
};
bool Chunk::isFlagged(int x, int y){
    return x>=0&&x<size&&y>=0&&y<size&&flagged[x][y]==true;
};
void Chunk::draw(BasicShader& shader){
    shader.loadModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(MapController::CHUNK_SIZE*x,0,MapController::CHUNK_SIZE*y)));
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, openCellCount*CELL_VERTICES_LENGTH, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(uncovered_cell_vao);
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(!isOpen(i, j)){
                shader.loadModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(i+MapController::CHUNK_SIZE*x,0,j+MapController::CHUNK_SIZE*y)));
                glDrawElements(GL_TRIANGLES, UNCOVERED_CELL_INDICES_LENGTH, GL_UNSIGNED_INT, (GLvoid *)0);
            }
        }
    }
    glBindVertexArray(flag_vao);
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            if(isFlagged(i, j)){
                shader.loadModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(i+MapController::CHUNK_SIZE*x,0,j+MapController::CHUNK_SIZE*y)));
                glDrawElements(GL_TRIANGLES, FLAG_INDICES_LENGTH, GL_UNSIGNED_INT, (GLvoid *)0);
            }
        }
    }
    glBindVertexArray(0);
};
void Chunk::unloadModels(){
    if(loadedModels){
        glDeleteVertexArrays(1, &uncovered_cell_vao);
        glDeleteBuffers(1, &uncovered_cell_vbo);
        glDeleteBuffers(1, &uncovered_cell_ebo);

        glDeleteVertexArrays(1, &flag_vao);
        glDeleteBuffers(1, &flag_vbo);
        glDeleteBuffers(1, &flag_ebo);
    }
};
