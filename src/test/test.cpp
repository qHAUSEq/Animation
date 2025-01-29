#include "../Animation.h"

using namespace graphics;

int main() {
    Logger::getInstance().setLogFile("log.txt");
	sf::RenderWindow window;
	window.create(sf::VideoMode::getDesktopMode(), "Test Animation", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Animation cloud("Texture\\SmallCloud_0", "SmallCloud_0", sf::Vector2f(400, 0), 0.001);
    cloud.setStartPosition(sf::Vector2f(100, 0));

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case event.Closed:
                window.close();
                break;
            }
        }
        cloud.update();
        cloud.draw(window);
        window.display();
        window.clear();
    }

	return 0;
}