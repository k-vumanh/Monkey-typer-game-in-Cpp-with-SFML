#include <iostream>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <string>
#include <fstream>

const int stateMenu = 1;
const int statePlay = 2;
const int stateSettings = 3;
const int stateGameOver = 4;
const int stateHighScores = 5;

class Game {
public:
    Game();

    void run();

private:
    void eventHandler();

    void update(float deltaTime);

    void redraw();

    void initializeWords();

    void typingHandler(const sf::Event &event);

    void spawnWords();

    void menuEventHandler(const sf::Event &event);

    void settingsEventHandler(const sf::Event &event);

    void loadFonts();

    void gameOverHandler(const sf::Event &event);

    void highScoresEventHandler(const sf::Event &event);


    void saveHighScore(const std::string &playerName, int playerScore);

    void loadHighScore();

    void displayHighScore();

    void enterPlayerName(const sf::Event &event);

    void inGameBackHandler(const sf::Event &event);

    sf::RenderWindow window;

    sf::Font font;
    sf::Font inputBoxFont;
    sf::Font font3;

    std::vector<sf::Text> words;
    sf::Clock clock;
    sf::Time wordsSpawnInterval;
    sf::Time timeSinceLastSpawn;

    std::string currentInput;
    std::string playerNameInput;

    int missedWords;
    int maxMissedWords;
    int score;
    float speedIncrease;

    float wordSize;

    sf::Text arrow;
    sf::Text inputBox;
    sf::Text scoreText;
    sf::Text missedText;
    sf::RectangleShape inGameBackButton;
    sf::Text inGameBackText;
    sf::RectangleShape line;
    sf::RectangleShape line2;
    sf::RectangleShape lineVertical;

    sf::RectangleShape playButton;
    sf::RectangleShape highScoresButton;
    sf::RectangleShape settingsButton;
    sf::RectangleShape exitButton;
    sf::Text playText;
    sf::Text highScoresText;
    sf::Text settingsText;
    sf::Text exitText;

    sf::RectangleShape backButton;
    sf::Text backText;

    sf::Text top15Text;

    sf::Text gameOverText;
    sf::Text enterNameText;

    std::vector<std::pair<std::string, int>> highScores;

    std::vector<sf::Text> fontOptions;
    std::vector<sf::Font> fonts;
    int selectedFontIndex;
    sf::Text fontIndicator;
    sf::RectangleShape slider;
    sf::RectangleShape sliderBar;
    sf::Text wordSizeText;

    int gameState;

    static const int FPS = 60;

    std::vector<float> wordsYPos;
    int currentYPos;

    std::vector<std::string> wordList;

};

Game::Game() :
        window(sf::VideoMode(900, 600), "Monkey typing"),
        wordsSpawnInterval(sf::seconds(1.5f)),
        timeSinceLastSpawn(sf::seconds(0)),
        gameState(stateMenu),
        missedWords(-1),
        maxMissedWords(5),
        score(0),
        wordSize(30.0f),
        speedIncrease(1.0f),
        currentYPos(0),
        selectedFontIndex(0) {


    if (!font.loadFromFile("fonts/Crang.ttf")) {
        fmt::println("Failed to load font from path");
        exit(-1);
    }


    if (!inputBoxFont.loadFromFile("fonts/retro.ttf")) {
        fmt::println("Failed to load inputBoxFont from path ");
        exit(-2);
    }


    if (!font3.loadFromFile("fonts/Daydream.ttf")) {
        fmt::println("Failed to load font3 from path");
        exit(-3);
    }

    wordsYPos = {50.0f, 100.0f, 150.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f};

    wordList = {"apple", "banana", "roof", "chisel", "butter", "water", "bottle", "great", "funny", "grandpa",
                "winter", "jungle", "keyboard", "space", "pencil", "mountain", "window", "guitar", "river",
                "forest", "lamp", "phone", "notebook", "table", "carpet", "camera", "radio", "piano", "violin",
                "bicycle", "engine", "garden", "bridge", "airport", "submarine", "satellite", "volcano", "desert",
                "hurricane", "tornado", "earthquake", "tsunami", "avalanche", "blizzard", "meteor", "galaxy", "push",
                "pull", "legs"};

    initializeWords();
    loadFonts();
    loadHighScore();

    inputBox.setFont(inputBoxFont);
    inputBox.setCharacterSize(20);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition(30, window.getSize().y - 30);

    arrow.setFont(inputBoxFont);
    arrow.setCharacterSize(20);
    arrow.setFillColor(sf::Color::Green);
    arrow.setPosition(10, window.getSize().y - 30);
    arrow.setString(">");

    scoreText.setFont(inputBoxFont);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Cyan);
    scoreText.setPosition(750, window.getSize().y - 60);
    scoreText.setString("SCORE: 0");

    missedText.setFont(inputBoxFont);
    missedText.setCharacterSize(20);
    missedText.setFillColor(sf::Color::Red);
    missedText.setPosition(600, window.getSize().y - 60);
    missedText.setString("MISSED: 0");

    line.setSize(sf::Vector2f(window.getSize().x, 2));
    line.setFillColor(sf::Color::White);
    line.setPosition(0, window.getSize().y - 60);

    line2.setSize(sf::Vector2f(window.getSize().x, 2));
    line2.setFillColor(sf::Color::White);
    line2.setPosition(0, window.getSize().y - 38);


    lineVertical.setSize(sf::Vector2f(2, 20));
    lineVertical.setFillColor(sf::Color::White);
    lineVertical.setPosition(740, window.getSize().y - 58);

    playButton.setSize(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Blue);
    playButton.setPosition(350, 150);

    highScoresButton.setSize(sf::Vector2f(220, 50));
    highScoresButton.setFillColor(sf::Color::Blue);
    highScoresButton.setPosition(340, 250);

    settingsButton.setSize(sf::Vector2f(200, 50));
    settingsButton.setFillColor(sf::Color::Blue);
    settingsButton.setPosition(350, 350);

    exitButton.setSize(sf::Vector2f(200, 50));
    exitButton.setFillColor(sf::Color::Blue);
    exitButton.setPosition(350, 450);

    backButton.setSize(sf::Vector2f(200, 50));
    backButton.setFillColor(sf::Color::Blue);
    backButton.setPosition(10, 10);

    inGameBackButton.setSize(sf::Vector2f(150, 40));
    inGameBackButton.setFillColor(sf::Color::Blue);
    inGameBackButton.setPosition(window.getSize().x - 150, 0);

    playText.setFont(font3);
    playText.setString("PLAY");
    playText.setCharacterSize(20);
    playText.setFillColor(sf::Color::Red);
    playText.setPosition(playButton.getPosition().x + 60, playButton.getPosition().y + 10);

    highScoresText.setFont(font3);
    highScoresText.setString("HIGH SCORES");
    highScoresText.setCharacterSize(20);
    highScoresText.setFillColor(sf::Color::Red);
    highScoresText.setPosition(highScoresButton.getPosition().x + 3, highScoresButton.getPosition().y + 10);

    settingsText.setFont(font3);
    settingsText.setString("SETTINGS");
    settingsText.setCharacterSize(20);
    settingsText.setFillColor(sf::Color::Red);
    settingsText.setPosition(settingsButton.getPosition().x + 20, settingsButton.getPosition().y + 10);


    exitText.setFont(font3);
    exitText.setString("EXIT");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::Red);
    exitText.setPosition(exitButton.getPosition().x + 60, exitButton.getPosition().y + 10);

    backText.setFont(font3);
    backText.setString("BACK");
    backText.setCharacterSize(20);
    backText.setFillColor(sf::Color::Red);
    backText.setPosition(backButton.getPosition().x + 60, backButton.getPosition().y + 10);

    inGameBackText.setFont(font3);
    inGameBackText.setString("PAUSE");
    inGameBackText.setCharacterSize(20);
    inGameBackText.setFillColor(sf::Color::Red);
    inGameBackText.setPosition(inGameBackButton.getPosition().x + 20, inGameBackButton.getPosition().y + 5);

    top15Text.setFont(font3);
    top15Text.setString("TOP 15 PLAYERS");
    top15Text.setCharacterSize(30);
    top15Text.setFillColor(sf::Color::Red);
    top15Text.setPosition(window.getSize().x / 3.5, 40);

    fontIndicator.setFont(inputBoxFont);
    fontIndicator.setString(">");
    fontIndicator.setCharacterSize(20);
    fontIndicator.setFillColor(sf::Color::Red);
    fontIndicator.setPosition(5, 100);

    for (int i = 0; i < fonts.size(); ++i) {
        sf::Text option;
        option.setFont(fonts[i]);
        option.setString("FONT " + std::to_string(i + 1));
        option.setCharacterSize(20);
        option.setFillColor(sf::Color::White);
        option.setPosition(20, 100 + i * 40);
        fontOptions.push_back(option);
    }

    sliderBar.setSize(sf::Vector2f(200, 10));
    sliderBar.setFillColor(sf::Color::White);
    sliderBar.setPosition(600, 200);

    slider.setSize(sf::Vector2f(10, 30));
    slider.setFillColor(sf::Color::Red);
    slider.setPosition(600 + (wordSize - 20) * 10, 190);

    wordSizeText.setFont(font);
    wordSizeText.setCharacterSize(24);
    wordSizeText.setFillColor(sf::Color::White);
    wordSizeText.setPosition(600, 150);
    wordSizeText.setString("Word Size: " + std::to_string(static_cast<int>(wordSize)));

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over! Enter your name:");
    gameOverText.setPosition(100, 200);

    enterNameText.setFont(font);
    enterNameText.setCharacterSize(30);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setPosition(150, 300);

}

void Game::run() {
    sf::Clock updateClock;
    while (window.isOpen()) {
        eventHandler();

        if (gameState == statePlay) {
            float deltaTime = updateClock.restart().asSeconds();
            update(deltaTime);
        } else {
            updateClock.restart();
        }

        redraw();

        sf::Time frameTime = sf::seconds(1.0f / FPS);
        sf::Time sleepTime = frameTime - updateClock.getElapsedTime();
        if (sleepTime.asMilliseconds() > 0) {
            sf::sleep(sleepTime);
        }
    }
}

void Game::eventHandler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (gameState == stateMenu) {
            menuEventHandler(event);
        } else if (gameState == statePlay && event.type == sf::Event::TextEntered) {
            typingHandler(event);
        } else if (gameState == statePlay && event.type == sf::Event::MouseButtonPressed) {
            inGameBackHandler(event);
        } else if (gameState == stateGameOver && event.type == sf::Event::TextEntered) {
            enterPlayerName(event);
        } else if (gameState == stateGameOver && event.type == sf::Event::MouseButtonPressed) {
            gameOverHandler(event);
        } else if (gameState == stateHighScores) {
            highScoresEventHandler(event);
        } else if (gameState == stateSettings) {
            settingsEventHandler(event);
        }

    }
}

void Game::update(float deltaTime) {
    timeSinceLastSpawn += sf::seconds(deltaTime);

    speedIncrease = 1.0f + 0.05f * score;


    for (auto it = words.begin(); it != words.end();) {
        if (it->getString() == currentInput) {
            score++;
            scoreText.setString("SCORE: " + std::to_string(score));
            currentInput.clear();
            inputBox.setString(currentInput);
            it = words.erase(it);
        } else {
            ++it;
        }
    }

    for (auto &word: words) {
        word.move(100 * deltaTime * speedIncrease, 0);
    }

    if (timeSinceLastSpawn >= wordsSpawnInterval) {
        spawnWords();
        timeSinceLastSpawn = sf::seconds(0);
    }

    for (auto it = words.begin(); it != words.end();) {
        if (it->getPosition().x > window.getSize().x) {
            missedWords++;
            missedText.setString("MISSED: " + std::to_string(missedWords));
            if (missedWords >= maxMissedWords) {
                fmt::print("Game Over! Too many missed words.\n");
                gameState = stateGameOver;
                break;
            }
            it = words.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::redraw() {
    window.clear(sf::Color::Black);

    if (gameState == stateMenu) {
        loadHighScore();
        std::sort(highScores.begin(), highScores.end(),
                  [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                      return b.second < a.second;
                  });
        window.draw(playButton);
        window.draw(playText);
        window.draw(highScoresButton);
        window.draw(highScoresText);
        window.draw(settingsButton);
        window.draw(settingsText);
        window.draw(exitButton);
        window.draw(exitText);
    } else if (gameState == statePlay) {
        for (const auto &word: words) {
            window.draw(word);
        }
        window.draw(line);
        window.draw(line2);
        window.draw(lineVertical);
        window.draw(arrow);
        window.draw(inputBox);
        window.draw(scoreText);
        window.draw(missedText);
        window.draw(inGameBackButton);
        window.draw(inGameBackText);
    } else if (gameState == stateSettings) {
        window.draw(backButton);
        window.draw(backText);
        for (const auto &option: fontOptions) {
            window.draw(option);
        }
        window.draw(fontIndicator);
        window.draw(sliderBar);
        window.draw(slider);
        window.draw(wordSizeText);
    } else if (gameState == stateGameOver) {
        window.draw(gameOverText);
        window.draw(enterNameText);
    } else if (gameState == stateHighScores) {
        window.draw(top15Text);
        window.draw(backButton);
        window.draw(backText);
        displayHighScore();
    }

    window.display();
}

void Game::initializeWords() {

    spawnWords();
}

void Game::spawnWords() {
    if (currentYPos >= wordsYPos.size()) {
        currentYPos = 0;
    }
    sf::Text word;
    word.setFont(fonts[selectedFontIndex]);

    std::string randomWords = wordList[rand() % wordList.size()];

    word.setString(randomWords);
    word.setCharacterSize(static_cast<int>(wordSize));
    word.setFillColor(sf::Color::White);
    word.setPosition(-100 - wordSize * 4, wordsYPos[currentYPos]);
    words.push_back(word);

    currentYPos++;
}

void Game::typingHandler(const sf::Event &event) {
    if (event.text.unicode < 128) {
        char enteredChar = static_cast<char>(event.text.unicode);
        if (enteredChar == '\b' && !currentInput.empty()) {
            currentInput.pop_back();
        } else if (enteredChar == '\r' || enteredChar == '\n') {
            currentInput.clear();
        } else if (enteredChar != '\b') {
            currentInput += enteredChar;
        }


        inputBox.setString(currentInput);
    }
}

void Game::menuEventHandler(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = statePlay;
        } else if (highScoresButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateHighScores;
        } else if (settingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateSettings;
        } else if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            window.close();
        }
    }
}

void Game::settingsEventHandler(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateMenu;
        }

        for (int i = 0; i < fontOptions.size(); ++i) {
            if (fontOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                selectedFontIndex = i;

                fontIndicator.setPosition(5, fontOptions[i].getPosition().y);
            }
        }

        if (sliderBar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            float newSliderPos = mousePos.x - sliderBar.getPosition().x;
            slider.setPosition(sliderBar.getPosition().x + newSliderPos, slider.getPosition().y);
            wordSize = 20 + newSliderPos / 10;
            wordSizeText.setString("Word Size: " + std::to_string(static_cast<int>(wordSize)));
        }
    }
}

void Game::loadFonts() {
    std::vector<std::string> fontPaths = {
            "fonts/retro.ttf",
            "fonts/Crang.ttf",
            "fonts/retro.ttf",
            "fonts/Daydream.ttf",
            "fonts/airstrike.ttf",
            "fonts/alagard.ttf",
            "fonts/BLADRMF_.TTF",
            "fonts/DS-DIGIT.TTF",
            "fonts/Dune_Rise.ttf",
            "fonts/MINECRAFT.ttf"

    };

    for (const auto &path: fontPaths) {
        sf::Font font;
        if (!font.loadFromFile(path)) {
            fmt::println("Failed to load font from path: {}", path);
        } else {
            fonts.push_back(font);
        }
    }
}

void Game::gameOverHandler(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateMenu;
        }
    }
}

void Game::highScoresEventHandler(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateMenu;
        }
    }
}

void Game::enterPlayerName(const sf::Event &event) {
    if (event.text.unicode < 128) {
        char enteredChar = static_cast<char>(event.text.unicode);
        if (enteredChar == '\b' && !playerNameInput.empty()) {
            playerNameInput.pop_back();
        } else if (enteredChar == '\r' || enteredChar == '\n') {
            saveHighScore(playerNameInput, score);
            playerNameInput.clear();
            gameState = stateMenu;
            window.close();
        } else if (enteredChar != '\b') {
            playerNameInput += enteredChar;
        }

        enterNameText.setString(playerNameInput);
    }
}

void Game::saveHighScore(const std::string &playerName, int playerScore) {

    highScores.push_back(std::make_pair(playerName, playerScore));

    std::sort(highScores.begin(), highScores.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return b.second > a.second;
              });

    if (highScores.size() > 15) {
        highScores.resize(15);
    }

    std::fstream outFile("highscores.txt");
    if (outFile.is_open()) {
        for (const auto &score: highScores) {
            outFile << score.first << " " << score.second << "\n";
        }

        outFile.close();
    } else {
        fmt::println("Error: unable to open file for writing");
    }
}

void Game::loadHighScore() {
    std::fstream inFile("highscores.txt");
    highScores.clear();
    std::string name;
    int score;
    while (inFile >> name >> score) {
        highScores.push_back(std::make_pair(name, score));
    }
    inFile.close();

    std::sort(highScores.begin(), highScores.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return b.second > a.second;
              });

    if (highScores.size() > 15) {
        highScores.resize(15);
    }
}

void Game::displayHighScore() {
    float firstY = 100.f;
    for (int i = 0; i < highScores.size(); ++i) {
        sf::Text scoreText;
        std::string place = std::to_string(i + 1);
        scoreText.setFont(inputBoxFont);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(300, firstY + i * 30);
        scoreText.setString(
                place + ". " + highScores[i].first + " -> " + std::to_string(highScores[i].second) + " Words");
        window.draw(scoreText);
    }
}

void Game::inGameBackHandler(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (inGameBackButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameState = stateMenu;
        }
    }
}

int main() {

    Game game;
    game.run();

    return 0;


}
