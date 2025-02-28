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
#include <vector>

#include <grid/gridHelper.hpp>
#include <imgui-SFML.h>// for ImGui::SFML::* functions and SFML-specific overloads
#include <ostream>

constexpr float MaxSize = 250.0F;

namespace {
// NOLINTNEXTLINE
constexpr auto NumOfRows = 64;
// NOLINTNEXTLINE
void drawSettings(float &framesBetweenUpdates, float &gridSize, bool &isPaused)
{
  ImGui::Begin("Settings menu");
  if (ImGui::Button((isPaused) ? "UnPause" : "Pause")) { isPaused = !isPaused; }
  ImGui::Separator();
  ImGui::SliderFloat("Enter grid size", &gridSize, 3, MaxSize, "%.3f", ImGuiSliderFlags_Logarithmic);
  constexpr auto MaxNumBetweenFrames = 2000;
  constexpr auto MinNumBetweenFrames = 0.1F;
  ImGui::SliderFloat("How many frames between update",
    &framesBetweenUpdates,
    MinNumBetweenFrames,
    MaxNumBetweenFrames,
    "%.3f",
    ImGuiSliderFlags_Logarithmic);
  ImGui::End();
}
void updateGrid(std::vector<std::vector<bool>> &grid)
{
  const Grid LocalGrid = Grid{ grid };
  for (std::size_t Row = 0; Row < LocalGrid.numOfRows(); Row++) {
    for (std::size_t Column = 0; Column < LocalGrid.numOfColumns(); Column++) {
      auto NumOfNeighbours = countNeighbours(LocalGrid, Row, Column);
      if (NumOfNeighbours == 3) {
        grid[Row][Column] = true;
      } else if (NumOfNeighbours == 2) {
        // DO nothing
      } else {
        grid[Row][Column] = false;
      }
    }
  }
}
void drawGrid(std::vector<std::vector<bool>> &Set, float const &gridSize)
{
  constexpr auto WidthToOutline = 0.05F;
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
    ImVec2(std::floor(gridSize * WidthToOutline) + 1, std::floor(gridSize * WidthToOutline) + 1));
  for (std::size_t Row = 0; Row < std::size(Set); Row++) {
    for (std::size_t Col = 0; Col < std::size(Set[Row]); Col++) {
      ImGui::PushID(static_cast<int>((Row * std::size(Set[Row])) + Col));

      // Change button color based on cell state.
      if (Set[Row][Col]) {
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
        Set[Row][Col].flip();// Toggle cell state when clicked
      }

      ImGui::PopStyleColor(3);// Restore button colors
      ImGui::PopID();

      // Place buttons in the same row, except after the last column
      if (Col < std::size(Set[Row]) - 1) { ImGui::SameLine(); }
    }
  }
}

// NOLINTNEXTLINE
void drawGrid()
{
  // NOLINTNEXTLINE
  static auto func = [gridSize = 100.0F,
                       Set = std::vector<std::vector<bool>>(NumOfRows, std::vector<bool>(NumOfRows, false)),
                       FramesSinceUpdate = 0.0F,
                       // NOLINTNEXTLINE
                       FramesBetweenUpdates = 60.0F,
                       IsPaused = true]() mutable {
    while (FramesSinceUpdate >= FramesBetweenUpdates && !IsPaused) {
      updateGrid(Set);
      FramesSinceUpdate -= FramesBetweenUpdates;
    }
    ImGui::Begin("Game window");

    // Set equal horizontal and vertical spacing between items
    {
      bool Expand = false;
      for (auto const &Elem : Set[0]) {
        if (Elem) { Expand = true; }
      }
      if (Expand) {
        Set.emplace(std::begin(Set), std::size(Set[0]), false);
        ImGui::SetScrollY(ImGui::GetScrollY() + gridSize);// offset scroll by one row}
      }
    }
    {
      bool Expand = false;
      for (auto const &Elem : *(std::end(Set) - 1)) {
        if (Elem) { Expand = true; }
      }
      if (Expand) { Set.emplace_back(std::size(Set[0]), false); }
    }
    {
      bool Expand = false;
      for (auto const &Row : Set) {
        if (Row[0]) { Expand = true; }
      }
      if (Expand) {
        Set.emplace(std::begin(Set), std::size(Set[0]), false);
        ImGui::SetScrollX(ImGui::GetScrollX() + gridSize);// offset scroll by one row}
      }
    }
    {
      bool Expand = false;
      for (auto const &Row : Set) {
        if (*(std::end(Row) - 1)) { Expand = true; }
      }
      if (Expand) { Set.emplace_back(std::size(Set[0]), false); }
    }

    drawGrid(Set, gridSize);
    if (!IsPaused) { FramesSinceUpdate++; }

    ImGui::PopStyleVar();

    ImGui::End();
    drawSettings(FramesBetweenUpdates, gridSize, IsPaused);
  };
  func();
}
}// namespace

int main()
{
  try {
    sf::Vector2u screenResolution = { 640, 480 };// NOLINT
    sf::RenderWindow Window(sf::VideoMode{ screenResolution }, "ImGui + SFML = <3");
    const auto FrameRate = 60;
    Window.setFramerateLimit(FrameRate);

    if (!ImGui::SFML::Init(Window)) { return -1; }
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;// NOLINT

    sf::CircleShape Shape(100.F);
    Shape.setFillColor(sf::Color::Green);

    sf::Clock DeltaClock;
    while (Window.isOpen()) {
      while (const auto Event = Window.pollEvent()) {
        ImGui::SFML::ProcessEvent(Window, *Event);

        if (Event->is<sf::Event::Closed>()) { Window.close(); }
      }

      ImGui::SFML::Update(Window, DeltaClock.restart());
      ImGui::DockSpaceOverViewport();

      drawGrid();
      Window.clear();
      Window.draw(Shape);
      ImGui::SFML::Render(Window);
      Window.display();
    }

    ImGui::SFML::Shutdown();
  } catch (std::exception &Err) {
    std::cerr << Err.what();
  }
}
