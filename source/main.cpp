#include<glm/glm.hpp>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<GL/glew.h>
#include "BasicShader.h"
#include "Player.h"
#include "Chunk.h"
#include<glm/gtc/matrix_transform.hpp>
#include<ctime>
#include<MapController.h>

int main(){
    srand((unsigned)time(0));
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "sfml", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    glewExperimental = GL_TRUE;
    glewInit();

    float aspectRatio = (float)window.getSize().x/window.getSize().y;
    sf::Vector2i center = sf::Vector2i(window.getSize().x/2, window.getSize().y/2);
    sf::Mouse::setPosition(center);
    BasicShader shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");

    Player player(0.5f, 0.5f, glm::radians(70.0f), aspectRatio, 0.1f, 100.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    sf::Image img;
    img.loadFromFile("textures/tiles.png");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    MapController mapController;
    mapController.openCell(0,0);
    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed)
                window.close();
            else if(e.type == sf::Event::KeyPressed)
                switch(e.key.code){
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        break;
                    default:
                        break;
                }
        }
        if(window.hasFocus()){
            player.rotateY((sf::Mouse::getPosition().x-center.x)*0.005f);
            player.rotateX((sf::Mouse::getPosition().y-center.y)*0.005f);

            sf::Mouse::setPosition(center);
            glm::vec2 movement(0.0f,0.0f);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
                movement.y+=1.0f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
                movement.y-=1.0f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
                movement.x+=1.0f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
                movement.x-=1.0f;
            }
            player.move(1.0f/60, mapController, movement);
        }
        glClearColor(0.0f,0.0f,0.0f,1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glm::vec2 lookingAt = player.getCenterRayContactPointWithPlane(0.2f);
        lookingAt.x = (int)std::floor(lookingAt.x);
        lookingAt.y = (int)std::floor(lookingAt.y);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            mapController.openCell(lookingAt.x, lookingAt.y); 
        }else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
                mapController.unflagCell(lookingAt.x, lookingAt.y);
            else
                mapController.flagCell(lookingAt.x, lookingAt.y);
        }
        shader.use();
        shader.loadViewMatrix(player.getViewMatrix());
        shader.loadProjectionMatrix(player.getProjectionMatrix());
        shader.loadLight(glm::vec3(player.getX(), 2.0f, player.getZ()));
        mapController.draw(shader);
        glUseProgram(0);

        window.display();
    }
    mapController.saveChunks();
    Chunk::unloadModels();

    return 0;
}
