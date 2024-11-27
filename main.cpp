#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>


class Circle : public sf::CircleShape
{
 public:


	 Circle(float radius, const std::string& inputName, const sf::Vector2f& position, const sf::Vector2f& inputVelocity, const sf::Color& color)
		 : sf::CircleShape(radius), name(inputName), velocity(inputVelocity)
	 {
		 setPosition(position);
		 setFillColor(color);
	 }


	 void setName(const std::string& inputName)
	 {
		 name = inputName;
	 }

	 std::string getName() const
	 {
		 return name;
	 }

	 void setVelocity(const sf::Vector2f& inputVelocity)
	 {
		 velocity = inputVelocity;
	 }

	 sf::Vector2f getVelocity() const
	 {
		 return velocity;
	 }

 private:
	std::string name;
	sf::Vector2f velocity;

};

class Rectangle : public sf::RectangleShape
{
public: 
	Rectangle(const std::string& inputName, const sf::Vector2f& position, const sf::Vector2f& inputVelocity, const sf::Color& color, const sf::Vector2f& rectangleSize)
		: sf::RectangleShape(rectangleSize), name(inputName), velocity(inputVelocity)  
	{
		setPosition(position);
		setFillColor(color);
	}

	void setName(const std::string& inputName)
	{
		name = inputName;
	}

	std::string getName() const
	{
		return name;
	}

	void setVelocity(const sf::Vector2f& inputVelocity)
	{
		velocity = inputVelocity;
	}

	sf::Vector2f getVelocity() const
	{
		return velocity;
	}

private:
	std::string name;
	sf::Vector2f velocity;
};

int main()
{
	std::ifstream configFile("bin/config.txt");

	std::string temp;

	 float wWidth = 0;
	 float wHeight = 0;

	 std::string fontPath;
	 int fontSize;

	

	 std::vector<Circle> circles;
	 circles.reserve(10);

	 std::vector<Rectangle> rectangles;
	 rectangles.reserve(10);


	while (configFile >> temp)
	{
		if (temp == "Window")
		{
			configFile >> wWidth >> wHeight;
			
		}
		else if (temp == "Font")
		{
			configFile >> fontPath >> fontSize;
		}
		else if (temp == "Circle")
		{
			std::string shapeName;
			
			float posX, posY, velX, velY;
			int r, g, b, radius;

			configFile >> shapeName >> posX >> posY >> velX >> velY >> r >> g >> b >> radius;

			circles.emplace_back(radius, shapeName, sf::Vector2f(posX, posY), sf::Vector2f(velX, velY), sf::Color(r, g, b));


		}
		else if (temp == "Rectangle")
		{
			std::string shapeName;

			float posX, posY, velX, velY;
			int r, g, b, rectWidth, rectHeight;

			configFile >> shapeName >> posX >> posY >> velX >> velY >> r >> g >> b >> rectWidth >> rectHeight;

			rectangles.emplace_back(shapeName, sf::Vector2f(posX, posY), sf::Vector2f(velX, velY), sf::Color(r, g, b), sf::Vector2f(rectWidth, rectHeight));
		}

	}

	sf::Vector2f windowParameters(wWidth, wHeight);

	
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Bouncing shapes!");

	//window.setFramerateLimit(10);


	sf::CircleShape circle(100);
	circle.setFillColor(sf::Color::Green); 
	circle.setPosition(300.0f, 300.0f);
	float circleMoveSpeed = 1.0f;

	sf::Font myFont;

	if (!myFont.loadFromFile("bin/fonts/BebasNeue-Regular.ttf"))
	{
		std::cerr << "Non è stato possibile caricare il font!\n";
		exit(-1);
	}


	while (window.isOpen())
	{

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}


		window.clear();

		for (auto& circle : circles)
		{
			circle.setPosition(circle.getPosition().x + circle.getVelocity().x, circle.getPosition().y + circle.getVelocity().y);
			sf::FloatRect boundingBox = circle.getGlobalBounds();
			sf::Vector2f invertedVelocity = circle.getVelocity();
			std::string circleName = circle.getName();
			sf::Text text(circleName, myFont, fontSize);
			bool velocityChanged = false;

			if (boundingBox.left < 0 || boundingBox.left + boundingBox.width > wWidth)
			{
				invertedVelocity.x = -invertedVelocity.x;
				velocityChanged = true;
				
			}

			if (boundingBox.top < 0 || boundingBox.top + boundingBox.height > wHeight)
			{
				invertedVelocity.y = -invertedVelocity.y;
				velocityChanged = true;
			}

			if (velocityChanged)
			{
				circle.setVelocity(invertedVelocity);
			}

			text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
			text.setPosition(circle.getPosition().x + circle.getRadius(), circle.getPosition().y + circle.getRadius());

			window.draw(circle);
			window.draw(text);
		}

		for (auto& rectangle : rectangles)
		{
			rectangle.setPosition(rectangle.getPosition().x + rectangle.getVelocity().x, rectangle.getPosition().y + rectangle.getVelocity().y);
			sf::FloatRect boundingBox = rectangle.getGlobalBounds();
			sf::Vector2f invertedVelocity = rectangle.getVelocity();
			std::string rectangleName = rectangle.getName();
			sf::Text text(rectangleName, myFont, fontSize);

			if (boundingBox.left < 0 || boundingBox.left + boundingBox.width > wWidth)
			{
				invertedVelocity.x = -invertedVelocity.x;
			}

			if (boundingBox.top < 0 || boundingBox.top + boundingBox.height > wHeight)
			{ 
				invertedVelocity.y = -invertedVelocity.y;

			}

			rectangle.setVelocity(invertedVelocity);

			text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
			text.setPosition(rectangle.getPosition() + (rectangle.getSize() / 2.f));
			window.draw(rectangle);
			window.draw(text);
		}

		
		window.display();

		
	}

	return 0;

}