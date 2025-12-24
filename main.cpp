#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;

random_device rd;
mt19937 gen(rd());

class point {
    private:
        bool alive = false;
    
    public:
        bool get_alive() {return alive;}
        char get_pic() {return (alive ? '#' : '.');}

        void start_init(size_t chance) {
            uniform_int_distribution<> dis(0, 99);
            int rand = dis(gen);
            alive = (rand < chance ? true : false);
        }

        void update(size_t birth, size_t death, size_t i, size_t j, vector<vector<point>>& matrix, vector<vector<point>>& second_matrix) {
            vector<pair<int, int>> neighbors = {{i-1, j-1}, {i-1, j}, {i-1, j+1},
                                                {i, j-1}, {i, j+1},
                                                {i+1, j-1}, {i+1, j}, {i+1, j+1}};
            size_t alive_neighbors = 0;
            for (auto& n : neighbors) {
                int ni = n.first;
                int nj = n.second;
                if (ni >= 0 && ni < matrix.size() && nj >= 0 && nj < matrix[0].size()) {
                    if (matrix[ni][nj].get_alive()) {alive_neighbors++;}
                }
            }
            if (alive) { //живая
                if (alive_neighbors < death) {
                    alive = false;
                }
            } else { //мертвая
                if (alive_neighbors > birth) {
                    alive = true;
                }
            }
        }
};

vector<vector<point>> matrix (20, vector<point>(20));
vector<vector<point>> second_matrix (20, vector<point>(20));

void print_matrix() {
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j].get_pic();
        }
        cout << endl;
    }
}

void start_init(size_t chance) {
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            matrix[i][j].start_init(chance);            
        }
    }
}

void next_iter(size_t birth, size_t death) {
    second_matrix = matrix;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            second_matrix[i][j].update(birth, death, i, j, matrix, second_matrix);            
        }
    }
    matrix = second_matrix;
}

size_t input_int(size_t min, size_t max) {
    size_t value;
    while (true) {
        cout << "Ввод > ";
        cin >> value;
        if (value < min || value > max) {
            cerr << "Ошибка: Введите значение в диапазоне от " << min << " до " << max << "!" << endl;
            cin.ignore(256, '\n');
            cin.clear();
        } else {
            break;
        }
    }
    return value;
}

void draw() {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(1000u, 1000u)), 
        "Window"
    );

    const float cell_size = 50.0f;

    while (window.isOpen()) {
        std::optional<sf::Event> event = window.pollEvent();
        
        window.clear(sf::Color::White);

        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));

                cell.setPosition(sf::Vector2f(
                    static_cast<float>(j) * cell_size,
                    static_cast<float>(i) * cell_size
                ));

                cell.setFillColor(matrix[i][j].get_alive() ? sf::Color::Black : sf::Color::White);

                window.draw(cell);
            }
        }

        window.display();
    }
}

int main() {
    size_t chance, birth_value, death_value;
    cout << "Введите шанс рождения клетки от 0 до 100" << endl;
    chance = input_int(0, 100);
    cout << "Введите предел рождения клетки от 0 до 8" << endl;
    birth_value = input_int(0, 8);
    cout << "Введите предел смерти клетки от 0 до 8" << endl;
    death_value = input_int(0, 8);

    start_init(chance);
    cout << "Сгенерированная матрица:" << endl;
    print_matrix();
    while (true) {
        cout << "Делаем ещё одну итерацию (1) или печатаем (2)?" << endl;
        size_t choose = input_int(1, 2);
        if (choose == 1) {
            next_iter(birth_value, death_value);
            cout << "Матрица на данный момент:" << endl;
            print_matrix();
        } else {
            break;
        }
    }
    draw();
}