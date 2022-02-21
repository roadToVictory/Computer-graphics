#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <math.h>
#include <iostream>

#include "font.h"

using namespace std;

class Text {
    public:
        Text() {
            _font.loadFromMemory(font_data, font_data_size);
            _text = new sf::Text;
            _text->setFont(_font);
            _text->setCharacterSize(12);
            _text->setFillColor(sf::Color::Black);
        }
		
		 ~Text() {
            if (_text)
                delete[] _text;
        }

        void outtextty(sf::RenderTarget& target, float x, float y, const char* str) const {
            _text->setPosition(x, y);
            _text->setString(str);
            target.draw(*_text);
        }

	private:
        sf::Font _font;
        sf::Text* _text;
};


class Circle : public sf::CircleShape, public Text {
    public:
        float x, y, z = .5f;
        char* str = new char[8];
        sf::Color blendColor;
        sf::BlendMode blendMode;
		
		Circle(float x, float y) : x(x), y(y) {
            texture->create(250,250);
            shape->setPosition(x, y);
            blender->setPosition(x, y);
            shape->setTexture(texture);
        }

        ~Circle() {
            delete colors_pixels;
            delete shape;
            delete blender;
            delete texture;
        }

        void generate_txtr(float z) {
            float r = 0; float a = 0;
            float ARZ[3]; float RGB[3];

            for (float i = -125; i < colors_size_x - 125; i++)
                for (float j = -125; j < colors_size_y - 125; j++) {
                    r = sqrtf(i * i + j * j);
                    a = fmodf(atan2f(j, i) * 57.29577951f + 180, 360);
                    if (r <= 125) {
                        ARZ[0] = a;
                        ARZ[1] = r * 0.008f;
                        ARZ[2] = z;
                        convert(ARZ, RGB);
                        draw_to_color_pixels(125 - static_cast<int>(i), 125 + static_cast<int>(j), RGB[0], RGB[1], RGB[2]);
                    }
                }
            texture->update(colors_pixels);
        }

        void updateBlend(float x) {
            z = x;
            blendColor.a = static_cast<sf::Uint8>(blendFactor() * 255);
            blender->setFillColor(blendColor);
        }

        void setBlend(sf::Color color) {
            blendColor = color;
            updateBlend(z);
        }

        void drawBlend(sf::RenderTarget& target) const {
            target.draw(*blender, sf::RenderStates(blendMode));
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(*shape);
            drawBlend(target);
        }

    private:
        const int colors_size_x = 250;
        const int colors_size_y = 250;
        sf::Uint8* colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
        sf::CircleShape* shape = new sf::CircleShape(125.0);
        sf::CircleShape* blender = new sf::CircleShape(125.0);
        sf::Texture* texture = new sf::Texture();

        inline void draw_to_color_pixels(unsigned int x, unsigned int y, float r, float g, float b){
            colors_pixels[4 * (y * colors_size_x + x) + 0] = static_cast<sf::Uint8>(r);
            colors_pixels[4 * (y * colors_size_x + x) + 1] = static_cast<sf::Uint8>(g);
            colors_pixels[4 * (y * colors_size_x + x) + 2] = static_cast<sf::Uint8>(b);
            colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
        }

        virtual void convert(float ARZ[3], float RGB[3]) = 0;   //funkcja wirtualna 
        virtual float blendFactor() = 0;

};


class HSL : public Circle {
    public:
        HSL(float x, float y) : Circle(x, y) {
            generate_txtr(0.5f);
            setBlend(sf::Color::White);
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            Circle::draw(target, states);
            sprintf_s(str, 8, "L=%.3f", z);
            outtextty(target, x, y + 6, "HSL");
            outtextty(target, x + 205, y + 240, str);
        }

    private:
        void convert(float HSL[3], float RGB[3]) {
            float a = (1.0f - fabsf(2.0f * HSL[2] - 1.0f)) * HSL[1];
            float b = a * (1.0f - fabsf(fmodf(HSL[0] * 0.016666f, 2.0f) - 1.0f));
            float h = HSL[2] - a / 2.0f;

            if (HSL[0] < 60) {
                RGB[0] = (a + h) * 255;
                RGB[1] = (b + h) * 255;
                RGB[2] = h * 255;
            }
            else if (HSL[0] < 120) {
                RGB[0] = (b + h) * 255;
                RGB[1] = (a + h) * 255;
                RGB[2] = h * 255;
            }
            else if (HSL[0] < 180) {
                RGB[0] = h * 255;
                RGB[1] = (a + h) * 255;
                RGB[2] = (b + h) * 255;
            }
            else if (HSL[0] < 240) {
                RGB[0] = h * 255;
                RGB[1] = (b + h) * 255;
                RGB[2] = (a + h) * 255;
            }
            else if (HSL[0] < 300) {
                RGB[0] = (b + h) * 255;
                RGB[1] = h * 255;
                RGB[2] = (a + h) * 255;
            }
            else {
                RGB[0] = (a + h) * 255;
                RGB[1] = h * 255;
                RGB[2] = (b + h) * 255;
            }

        }

        float blendFactor() {
            if (z <= 0.5f)
                blendMode = sf::BlendMode(sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::DstAlpha, sf::BlendMode::ReverseSubtract);
            else blendMode = sf::BlendAdd;
        
            return fabsf(z - 0.5f) * 2.0f;
        }
};

class HSV : public Circle {
    public:
        HSV(float x, float y) : Circle(x,y) {
            blendMode = sf::BlendAlpha;
            generate_txtr(1.0f);
            setBlend(sf::Color::Black);
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            Circle::draw(target, states);
            sprintf_s(str, 8, "V=%.3f", z);
            outtextty(target, x, y + 6, "HSV");
            outtextty(target, x + 205, y + 240, str);
        }

    private:
        void convert(float HSV[3], float RGB[3]) {
            float a = HSV[1]*HSV[2];
            float b = a * (1.0f - fabsf(fmodf(HSV[0] * 0.016666f, 2.0f) - 1.0f));
            float h = HSV[2] - a;

            if (HSV[0] < 60) {
                RGB[0] = (a + h) * 255;
                RGB[1] = (b + h) * 255;
                RGB[2] = h * 255;
            }
            else if (HSV[0] < 120) {
                RGB[0] = (b + h) * 255;
                RGB[1] = (a + h) * 255;
                RGB[2] = h * 255;
            }
            else if (HSV[0] < 180) {
                RGB[0] = h * 255;
                RGB[1] = (a + h) * 255;
                RGB[2] = (b + h) * 255;
            }
            else if (HSV[0] < 240) {
                RGB[0] = h * 255;
                RGB[1] = (b + h) * 255;
                RGB[2] = (a + h) * 255;
            }
            else if (HSV[0] < 300) {
                RGB[0] = (b + h) * 255;
                RGB[1] = h * 255;
                RGB[2] = (a + h) * 255;
            }
            else {
                RGB[0] = (a + h) * 255;
                RGB[1] = h * 255;
                RGB[2] = (b + h) * 255;
            }

        }

        float blendFactor() {
            return 1.0f - z;
        }
};

class CMY : public Circle {
public:
    CMY(float x, float y) : Circle(x, y) {
        blendMode = sf::BlendMode(sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::DstAlpha, sf::BlendMode::ReverseSubtract);
        generate_txtr(.0f);
        setBlend(sf::Color::Blue);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        Circle::draw(target, states);
        sprintf_s(str, 8, "Y=%.0f%%", z*100);   //to percent
        outtextty(target, x, y + 6, "CMY");
        outtextty(target, x + 205, y + 240, str);
    }

private:
    void convert(float CMY[3], float RGB[3]) {
        RGB[0] = (1 - CMY[1]) * 255;
        RGB[1] = (1 - CMY[0] * 0.002777f) * 255;
        RGB[2] = (1 - CMY[2]) * 255;
    }

    float blendFactor() {
        return z;
    }
};

class RGB : public Circle {
public:
    RGB(float x, float y) : Circle(x, y) {
        blendMode = sf::BlendAdd;
        generate_txtr(.0f);
        setBlend(sf::Color::Blue);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        Circle::draw(target, states);
        sprintf_s(str, 8, "B=%.0f", z * 255);   
        outtextty(target, x, y + 6, "RGB");
        outtextty(target, x + 205, y + 240, str);
    }

private:
    void convert(float R[3], float RGB[3]) {
        RGB[0] = R[1] * 255;
        RGB[1] = R[0] * 0.708333f;
        RGB[2] = R[2] * 255;
    }

    float blendFactor() {
        return z;
    }
};

class Drawer : public sf::Drawable, public Text {
    public:
        bool ifMousePressed = false;

        Drawer() {
            c1 = new HSL(25, 25);
            c2 = new HSV(325, 25);
            c3 = new CMY(25, 325);
            c4 = new RGB(325, 325);

            for(int i=2; i<scrollX; i++)
                for (int j = 1; j < scrollY; j++) 
                    draw_to_color_pixels(i, j, j);
                
            scrollTextures->create(scrollX, scrollY);
            scrollTextures->update(scrollPixels);

            scroll->setTexture(scrollTextures);
            scroll->setPosition(611, 10);
            scroll->setOutlineColor(sf::Color::Black);
            scroll->setOutlineThickness(1.0f);

            (*scrollPosition)[0].color = (*scrollPosition)[1].color = sf::Color::Black;
        }

        void act_z(float x) {
            z = x;
            c1->updateBlend(z);
            c2->updateBlend(z);
            c3->updateBlend(z);
            c4->updateBlend(z);
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(*c1);
            target.draw(*c2);
            target.draw(*c3);
            target.draw(*c4);
            target.draw(*scroll);

            (*scrollPosition)[0].position = sf::Vector2f(600, 11 + z * 255);
            (*scrollPosition)[1].position = sf::Vector2f(608, 11 + z * 255);
            target.draw(*scrollPosition);

            (*scrollPosition)[0].position = sf::Vector2f(651, 11 + z * 255);
            (*scrollPosition)[1].position = sf::Vector2f(659, 11 + z * 255);
            target.draw(*scrollPosition);
        }

        ~Drawer() {
            delete c1;
            delete c2;
            delete c3;
            delete c4;
            delete scroll;
            delete scrollPixels;
            delete scrollPosition;
            delete scrollTextures;
        }


    private:
        Circle* c1, * c2, * c3, * c4;
        const int scrollX = 37;
        const int scrollY = 256;
        float z = 0.5f;

        sf::RectangleShape* scroll = new sf::RectangleShape(sf::Vector2f(static_cast<float>(scrollX), static_cast<float>(scrollY)));
        sf::Uint8* scrollPixels = new sf::Uint8[scrollX * scrollY * 4];
        sf::Texture* scrollTextures = new sf::Texture();
        sf::VertexArray* scrollPosition = new sf::VertexArray(sf::Lines, 2);
        
        inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned int z) {
            scrollPixels[4 * (y * scrollX + x) + 0] = z;
            scrollPixels[4 * (y * scrollX + x) + 1] = z;
            scrollPixels[4 * (y * scrollX + x) + 2] = z;
            scrollPixels[4 * (y * scrollX + x) + 3] = 255;
        }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 600), "GFK Lab 02 - kolorowe koleczka", sf::Style::Close | sf::Style::Resize);
    sf::Clock clock;
    sf::Time time = sf::Time::Zero;
    unsigned int FPS = 0, frame_counter = 0; 
 
    Drawer drawer;
    sf::Event event;
    //inicjalizacja 
    clock.restart().asMilliseconds();
    int last_frame_time=0;
    wchar_t label[40];
    char FPS_text[10] = "0 FPS";
    //char time_text[10] = "0 s";

    while (window.isOpen()){
        
        window.clear(sf::Color::White);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) {
                    if (event.mouseButton.x >= 613 && event.mouseButton.x <= 648 && event.mouseButton.y >= 11 && event.mouseButton.y <= 266) {
                        drawer.act_z((event.mouseButton.y-11)/255.0f);
                        drawer.ifMousePressed = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (drawer.ifMousePressed) {
                    if (event.mouseMove.x >= 550 && event.mouseMove.x <= 700 && event.mouseMove.y >= 11 && event.mouseMove.y <= 266) {  //z lekkim zakresem "obok" slidera
                        drawer.act_z((event.mouseMove.y-11)/255.0f);
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) && drawer.ifMousePressed) {
                        drawer.ifMousePressed = false;
            }
        }
        
        last_frame_time = (unsigned int)(clock.getElapsedTime().asMicroseconds());
        swprintf(label, sizeof(label), L"%d μs", last_frame_time); 
        
        drawer.outtextty(window, 610, 310, (const char*)label);
        
        clock.restart();
        
        drawer.outtextty(window, 610, 317, FPS_text);
        window.draw(drawer);
        window.display();
    }
}