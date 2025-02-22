#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstddef>
#include <exception>
#include <imgui.h>// necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include <iostream>
#include <iterator>
#include <print>
#include <vector>

#include <grid/gridHelper.hpp>
#include <imgui-SFML.h>// for ImGui::SFML::* functions and SFML-specific overloads
#include <ostream>

constexpr float maxSize = 250.0F;

namespace {
// NOLINTNEXTLINE
constexpr auto NumOfRows = 64;
// NOLINTNEXTLINE
void drawGrid()
{
  // NOLINTNEXTLINE
  static auto func = [gridSize = 100.0F,
                       set = std::vector<std::vector<bool>>(NumOfRows, std::vector<bool>(NumOfRows, false)),
                       framesSinceUpdate = 0,
                       // NOLINTNEXTLINE
                       framesBetweenUpdates = 2'000'000,
                       isPaused = true]() mutable {
    if (framesSinceUpdate >= framesBetweenUpdates && !isPaused) {
      framesSinceUpdate = 0;
      const Grid grid = Grid{ set };
      for (std::size_t row = 0; row < grid.numOfRows(); row++) {
        for (std::size_t column = 0; column < grid.numOfColumns(); column++) {
          std::print("Number of neighbours at index {{{}, {}}} ", row, column);
          auto numOfNeighbours = countNeighbours(grid, row, column);
          std::print("is {}\n", numOfNeighbours);
          if (numOfNeighbours == 3) {
            set[row][column] = true;
          } else if (numOfNeighbours == 2) {
            // DO nothing
          } else {
            set[row][column] = false;
          }
        }
      }
    }
    ImGui::Begin("Hello, world!");
    if (ImGui::Button((isPaused) ? "UnPause" : "Pause")) { isPaused = !isPaused; }

    // Set equal horizontal and vertical spacing between items
    constexpr auto maxNumBetweenFrames = 2000;
    ImGui::SliderInt("How many frames between update", &framesBetweenUpdates, 1, maxNumBetweenFrames);
    constexpr auto widthToOutline = 0.05F;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
      ImVec2(std::floor(gridSize * widthToOutline) + 1, std::floor(gridSize * widthToOutline) + 1));

    // Loop over rows and columns to create the grid of buttons.
    for (std::size_t row = 0; row < std::size(set); row++) {
      for (std::size_t col = 0; col < std::size(set[row]); col++) {
        ImGui::PushID(static_cast<int>((row * std::size(set[row])) + col));

        // Change button color based on cell state.
        if (set[row][col]) {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0F, 1.0F, 1.0F, 1.0F));// White for active cell
        } else {
          // NOLINTNEXTLINE
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2F, 0.2F, 0.2F, 1.0F));// Dark grey for inactive cell
        }

        // Set hover and active colors
        // NOLINTNEXTLINE
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1));// Light grey hover
        // NOLINTNEXTLINE
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1));// Slightly darker when clicked

        // Create the button
        if (ImGui::Button("##cell", ImVec2(gridSize, gridSize))) {
          set[row][col].flip();// Toggle cell state when clicked
        }

        ImGui::PopStyleColor(3);// Restore button colors
        ImGui::PopID();

        // Place buttons in the same row, except after the last column
        if (col < std::size(set[row]) - 1) { ImGui::SameLine(); }
      }
    }
    if (!isPaused) { framesSinceUpdate++; }

    ImGui::PopStyleVar();
    ImGui::Separator();
    ImGui::SliderFloat("Enter grid size", &gridSize, 3, maxSize);

    ImGui::End();
  };
  func();
}
}// namespace

int main()
{
  try {
    sf::Vector2u screenResolution = { 640, 480 };// NOLINT
    sf::RenderWindow window(sf::VideoMode{ screenResolution }, "ImGui + SFML = <3");
    const auto frameRate = 60;
    window.setFramerateLimit(frameRate);

    if (!ImGui::SFML::Init(window)) { return -1; }
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;// NOLINT

    sf::CircleShape shape(100.F);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    ;
    while (window.isOpen()) {
      while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>()) { window.close(); }
      }

      ImGui::SFML::Update(window, deltaClock.restart());
      ImGui::DockSpaceOverViewport();

      drawGrid();
      window.clear();
      window.draw(shape);
      ImGui::SFML::Render(window);
      window.display();
    }

    ImGui::SFML::Shutdown();
  } catch (std::exception &Err) {
    std::cerr << Err.what();
  }
}
