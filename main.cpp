#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <thread>

const double PI = 3.14159265358979323846;
using namespace std::chrono_literals;
using std::cout;


double d2r(double degrees) {
    return degrees * PI / 180.0;
}

double getTime() {
    using namespace std::chrono;
    return duration_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count();
}

class Car 
{
private:
	std::pair <float, float> _cords; //first x , second y
	std::pair <float, float> _dest_cords = {300, 300};

	/**
	 * string (speed_vector name)
	 * pair (first - angle, second - speed) 
	 */
	std::unordered_map<std::string, std::pair<float, float>> speed_vectors;

	float _car_disired_angle = 0;
	float _width = 50;
	float _height = 25;
	float _max_speed = 5;
	float _back_max_speed = -5;
	float _accel = 1;
	float _angle_accel = 1;
public:
	sf::RectangleShape shape;

	const std::pair<float, float>& get_cord(){return _cords;};
	float get_width(){return _width;};
	float get_height(){return _height;};

	void calc_drive_speed(){
        bool A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		if (A && W)
		{
			speed_vectors["car"].first--;
			if (speed_vectors["car"].second < _max_speed)
				speed_vectors["car"].second += _accel;
		}
		else if (W && D) 
		{
			speed_vectors["car"].first++;
			if (speed_vectors["car"].second < _max_speed)
				speed_vectors["car"].second += _accel;
		}
		else if (A && S && speed_vectors["car"].second > 50)
		{
			speed_vectors["car"].first--;	// cuz move is backward 
			if (speed_vectors["car"].second > _back_max_speed)
				speed_vectors["car"].second -= _accel;
		}
		else if (A && S && speed_vectors["car"].second < -50)
		{
			speed_vectors["car"].first++;	// cuz move is backward 
			if (speed_vectors["car"].second > _back_max_speed)
				speed_vectors["car"].second -= _accel;
		}
		else if (S && D && speed_vectors["car"].second > 50) // cuz move is backward 
		{
			speed_vectors["car"].first++;
			if (speed_vectors["car"].second > _back_max_speed)
				speed_vectors["car"].second -= _accel;
		}
		else if (S && D && speed_vectors["car"].second < -50) // cuz move is backward 
		{
			speed_vectors["car"].first--;
			if (speed_vectors["car"].second > _back_max_speed)
				speed_vectors["car"].second -= _accel;
		}
		else if (A && speed_vectors["car"].second > 50) 
			speed_vectors["car"].first--;
		else if (A && speed_vectors["car"].second < -50) 
			speed_vectors["car"].first++;
		else if (S && speed_vectors["car"].second > _back_max_speed) 
			speed_vectors["car"].second -= _accel;
		else if (D && speed_vectors["car"].second > 50) 
			speed_vectors["car"].first++;
		else if (D && speed_vectors["car"].second < -50) 
			speed_vectors["car"].first--;
		else if (W && speed_vectors["car"].second < _max_speed) 
			speed_vectors["car"].second += _accel;
	};
	
	// float getspeed_vectors["car"].second()
	// {
	// 	return speed_vectors["car"].second;
	// };


	/*
	// void calc_autodrive_vector(){
	// 	if (_car_disired_angle - 90 > speed_vectors["car"].first)
	// 		speed_vectors["car"].first++;
	// 	else if (_car_disired_angle - 90 < speed_vectors["car"].first)
	// 		speed_vectors["car"].first--;
	// 	calc_desired_angle();
	// };

	// void calc_desired_angle()
	// {
	// 	std::pair<float, float> sum_vec = {_cords.first + _dest_cords.first, _cords.second + _dest_cords.second};

	// 	_car_disired_angle = std::atan2(sum_vec.second, sum_vec.first) * (180.0 / M_PI);
	// }

	// float speed_vectors["car"].first
	// {
	// 	return speed_vectors["car"].first;
	// };
	
	// void set_current_degree(float degree)
	// {
	// 	speed_vectors["car"].first = degree;
	// };
	
	// float getspeed_vectors["car"].second()
	// {
	// 	return speed_vectors["car"].second;
	// };
	
	// void setspeed_vectors["car"].second(float degree)
	// {
	// 	speed_vectors["car"].second = degree;
	// };
	*/

	void move_circle()
	{

	}

	void move()
	{	
		shape.setRotation(speed_vectors["car"].first);
		shape.setPosition(get_cord().first, get_cord().second);
		_cords.first += speed_vectors["car"].second * std::sin(d2r(speed_vectors["car"].first));
		_cords.second -= speed_vectors["car"].second * std::cos(d2r(speed_vectors["car"].first));
	}

	Car(std::pair<float, float> cords, float car_deggre, float wheel_deggree) : _cords(cords)
	{
		speed_vectors["car"] = {wheel_deggree, 0};
		speed_vectors["destination"] = {10, 0};
		speed_vectors["car_destination"] = {-10, 0};
		this->shape = sf::RectangleShape(sf::Vector2f(this->_height, this->_width)); 
	};

	void decress_speed(sf::RenderWindow &window, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
		{
			while (speed_vectors["car"].second != 0)
			{
				shape.setRotation(speed_vectors["car"].first);
				shape.setPosition(get_cord().first, get_cord().second);
				window.clear(sf::Color::Black);
				if (speed_vectors["car"].second > 0)
					speed_vectors["car"].second -= _accel;
				else 
					speed_vectors["car"].second += _accel;
				move();
				window.draw(shape);  
				window.display();
				window.clear();
			}
		}
	};
};

int main()
{
	Car car({100,500}, 0, 0);
	car.shape.setOrigin({car.get_height() / 2.f, car.get_width() / 2.f});

	sf::RenderWindow window(sf::VideoMode(800,800), "SFML window");
	sf::Event event;

	double last_time = getTime();

	while (window.isOpen())
	{

		window.clear(sf::Color::Black);
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				car.calc_drive_speed();
			if (event.type == sf::Event::KeyReleased)
			{
				// car.decress_speed(window, event);
				// car.calc_autodrive_vector();
			}
		}

		std::this_thread::sleep_for(10ms);
		car.move();
		window.draw(car.shape);  
		window.display();
		window.clear();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}
