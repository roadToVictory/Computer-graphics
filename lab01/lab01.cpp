#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "font.h"

#include <iostream>

class Menu : public sf::Drawable{
	
private:
	sf::RectangleShape* rectangle;
    sf::Font font;
    sf::Text* text;
    sf::Texture* texture_colors;
    sf::Sprite* sprite_colors;
    sf::Uint8* pixels_colors;
    const unsigned int col_size_x = 765;
    const unsigned int col_size_y = 60;
	
    inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b){
        pixels_colors[4 * (y * col_size_x + x) + 0] = r;
        pixels_colors[4 * (y * col_size_x + x) + 1] = g;
        pixels_colors[4 * (y * col_size_x + x) + 2] = b;
        pixels_colors[4 * (y * col_size_x + x) + 3] = 255;
    }

public:
    Menu(){
        text = new sf::Text;
        font.loadFromMemory(font_data, font_data_size);
        text->setFont(font);
        text->setCharacterSize(12);
        text->setFillColor(sf::Color::White);

        rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
        rectangle->setOutlineColor(sf::Color::White);
        rectangle->setOutlineThickness(1.0f);
        rectangle->setFillColor(sf::Color::Transparent);
        rectangle->setPosition(2, 62);

        unsigned int x, y;
        pixels_colors = new sf::Uint8[col_size_x * col_size_y * 4];
        for (x = 0; x < 255; x++)
            for (y = 0; y < 30; y++){
                draw_to_color_pixels(x, y, x, 255, 0);
                draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
                draw_to_color_pixels(x + 510, y, 255, 0, x);
                draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
                draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
                draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
            }

        texture_colors = new sf::Texture();
        texture_colors->create(col_size_x, col_size_y);
        texture_colors->update(pixels_colors);

        sprite_colors = new sf::Sprite();
        sprite_colors->setTexture(*texture_colors);
        sprite_colors->setPosition(1, 1);
    }

    sf::Image palette(){
        return texture_colors->copyToImage();
    }

    void outtextxy(sf::RenderTarget& target, float x, float y, const wchar_t* str) const{
        text->setString(str);
        text->setPosition(x, y);
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        outtextxy(target, 5, 600, L"f - wybór koloru rysowania");
        outtextxy(target, 5, 615, L"b - wybór koloru wypełniania");
        outtextxy(target, 5, 630, L"l - rysowanie linii");
        outtextxy(target, 200, 600, L"r - rysowanie prostokąta");
        outtextxy(target, 200, 615, L"a - rysowanie wypełnionego prostokata");
        outtextxy(target, 200, 630, L"c - rysowanie okręgu");
        outtextxy(target, 470, 600, L"w - zapis do pliku");
        outtextxy(target, 470, 615, L"o - odczyt z pliku");
        outtextxy(target, 470, 630, L"esc - wyjście");
        outtextxy(target, 650, 615, L"Aktualny wybór:");
        
        target.draw(*rectangle);
        target.draw(*sprite_colors);
    }
};

class Drawer : public sf::Drawable {    //główna klasa obsługująca polecenia uzytkownika
    public:
        sf::RectangleShape out_color, in_color;
        sf::Color draw_c, fill_c;
        char option = ' ';
        bool ifMouse;
        sf::Vector2f start, end;

        //obsluga bufora
        sf::RenderTexture TextureBuf;
        sf::Sprite SpriteBuf;

        //konstruktor domyslny ustawiajacy biezacy kolor rysowania i wypelnienia
        Drawer() {
			ifMouse = false;
            out_color.setSize(sf::Vector2f(32, 29));
            out_color.setOutlineColor(sf::Color::Transparent);
            out_color.setOutlineThickness(1.0);
            out_color.setFillColor(draw_c);
            out_color.setPosition(767, 1);      //powiekszanie

            in_color.setSize(sf::Vector2f(32, 29));
            in_color.setOutlineColor(sf::Color::Transparent);
            in_color.setOutlineThickness(0);
            in_color.setFillColor(fill_c);
            in_color.setPosition(767, 31);      

            TextureBuf.create(800, 600);
            SpriteBuf.setTexture(TextureBuf.getTexture());
            TextureBuf.clear();
            start = end = sf::Vector2f(0, 0);	//reset
        }

        //metoda przeciazona virtualna do rysowania ksztaltow
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            switch (option) {
                case 'l': {
                    sf::VertexArray line(sf::LinesStrip, 2);
                    line[0].position = start; //poczatek linii
                    line[0].color = draw_c;
                    line[1].position = end; //koniec linii
                    line[1].color = fill_c;
                    target.draw(line);  //narysowanie linii
                    break;
                }
                case 'a':
                case 'r': {	// dla "a" i "r"
                    sf::RectangleShape rectangle(sf::Vector2f(end.x - start.x, end.y - start.y));
                    rectangle.setPosition(start);
                    rectangle.setOutlineColor(draw_c);
                    rectangle.setOutlineThickness(1);

                    if (option == 'r')
                        rectangle.setFillColor(sf::Color::Transparent);
                    else rectangle.setFillColor(fill_c);
                    target.draw(rectangle);
                    break;
                }
                case 'c': {
                    sf::CircleShape circle((sqrt( pow(end.x-start.x,2)+ pow(end.y-start.y,2) )/2));
                    circle.setPosition((end.x - start.x) / 2 + start.x - (sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2)) / 2), start.y - (sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2)) / 2) + (end.y - start.y) / 2 );
                    circle.setRotation(atan2f(end.x - start.x, end.y - start.y));
                    circle.setOutlineThickness(1);
                    circle.setOutlineColor(draw_c);
                    circle.setFillColor(sf::Color::Transparent);
                    target.draw(circle);
                    break;
                }
            }
        }

};

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01 - cosik a'la Paint", sf::Style::Close | sf::Style::Resize);
    sf::Event event;
    Menu menu;
    //sf::Image palette = menu.palette();
    window.setFramerateLimit(60);   //limit klatek na sekunde FPS
    Drawer drawer;  
    
    while (window.isOpen()){
        window.clear(sf::Color::Black); //kolor tla

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) 
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {  //jeśli został wcisniety przycisk myszy
                if (event.mouseButton.button == sf::Mouse::Left) {  //wcisniety LPM
                    if ((drawer.option == 'b' || drawer.option == 'f') && (event.mouseButton.x <= 765 && event.mouseButton.y <= 60)) {
                        if (drawer.option == 'b') {
                            drawer.fill_c = menu.palette().getPixel(event.mouseButton.x, event.mouseButton.y);
                            drawer.in_color.setFillColor(drawer.fill_c);
                        }
                        else {
                            drawer.draw_c = menu.palette().getPixel(event.mouseButton.x, event.mouseButton.y);
                            drawer.out_color.setFillColor(drawer.draw_c);
                        }
                    }
                    else if (event.mouseButton.y <= 600 && event.mouseButton.y > 60) {
                        drawer.ifMouse = true;  //ustawienie flagi
                        drawer.start = sf::Vector2f(static_cast<float> (event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                        drawer.end = drawer.start;
                    }
                }
            }   //buttonpressed
            else{
                if(event.type == sf::Event::MouseMoved) {  //ruszanie myszka
                    if ( drawer.ifMouse && (event.mouseMove.x > 1 && event.mouseMove.x < 799 && event.mouseMove.y > 60 && event.mouseMove.y < 600))
                        drawer.end = sf::Vector2f(static_cast<float> (event.mouseMove.x), static_cast<float> (event.mouseMove.y));
                }
                else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && drawer.ifMouse) {   //puszczenie PM
                    drawer.ifMouse = false; //zdjecie flagi
                    drawer.TextureBuf.draw(drawer);
                    drawer.TextureBuf.display();
                    drawer.start = drawer.end = sf::Vector2f(0, 0);
                    drawer.ifMouse = false;
                }
                else {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape) 
                            window.close();         
                        else {
                            if(event.key.code == sf::Keyboard::O) {
                                sf::Texture importp;
                                importp.loadFromFile("myPicture.jpg");
                                drawer.TextureBuf.draw(sf::Sprite(importp));
                            }
                            else if (event.key.code == sf::Keyboard::W)
                                drawer.SpriteBuf.getTexture()->copyToImage().saveToFile("myPicture.jpg");
                            else {
                                switch (event.key.code) {
                                    case(sf::Keyboard::B): {
                                        drawer.option = 'b';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                    case(sf::Keyboard::F): {
                                        drawer.option = 'f';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                    case(sf::Keyboard::L): {
                                        drawer.option = 'l';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                    case(sf::Keyboard::R): {
                                        drawer.option = 'r';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                    case(sf::Keyboard::A): {
                                        drawer.option = 'a';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                    case(sf::Keyboard::C): {
                                        drawer.option = 'c';
                                        drawer.start = drawer.end = sf::Vector2f(0, 0);
                                        drawer.ifMouse = false;
                                        break;
                                    }
                                }
                            }
                        }
                }
            }
        }
        //Draw BEGIN
        wchar_t x[2] = {static_cast<wchar_t>(drawer.option), '\0'}; 
        
        window.draw(drawer.SpriteBuf);
        window.draw(drawer);
        window.draw(drawer.out_color);
        window.draw(drawer.in_color);
        menu.outtextxy(window, 750, 615, x);    //biezaca aktywna opcja (literka)
        window.draw(menu);
        window.display();
        //Draw END
    }
    //window.close();
    return 0;
}