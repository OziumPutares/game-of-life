#ifndef SAMPLE_LIBRARY_HPP
#define SAMPLE_LIBRARY_HPP

#include "gridClass.hpp"
#include <cstddef>
#include <myproject/sample_library_export.hpp>
#include <print>

[[nodiscard]] SAMPLE_LIBRARY_EXPORT int
  countNeighbours(Grid const & /*grid*/, std::size_t /*row*/, std::size_t /*column*/) noexcept;

// NOLINTNEXTLINE
[[nodiscard]] int countNeighbours(Grid const &grid, std::size_t row, std::size_t column) noexcept
{
  if (row == 0) {// handle 0 case
    if (column == 0) {// handle 0 case
      return static_cast<int>(grid.getRow(row + 1)[column + 1]) + static_cast<int>(grid.getRow(row)[column + 1])
             + static_cast<int>(grid.getRow(row + 1)[column]);
    }
    if (column == std::size(grid.getRow(row)) - 1) {// Handle size case
      return static_cast<int>(grid.getRow(row + 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
             + static_cast<int>(grid.getRow(row + 1)[column]);
    }
    return static_cast<int>(grid.getRow(row + 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
           + static_cast<int>(grid.getRow(row + 1)[column]) + static_cast<int>(grid.getRow(row + 1)[column + 1])
           + static_cast<int>(grid.getRow(row)[column + 1]);
  }
  if (row == grid.numOfRows() - 1) {// Handle size case
    if (column == 0) {// handle 0 case
      return static_cast<int>(grid.getRow(row - 1)[column + 1]) + static_cast<int>(grid.getRow(row)[column + 1])
             + static_cast<int>(grid.getRow(row - 1)[column]);
    }
    if (column == grid.numOfColumns() - 1) {// Handle size case
      return static_cast<int>(grid.getRow(row - 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
             + static_cast<int>(grid.getRow(row - 1)[column]);
    }
    std::println("Hello there");
    return static_cast<int>(grid.getRow(row - 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
           + static_cast<int>(grid.getRow(row - 1)[column]) + static_cast<int>(grid.getRow(row - 1)[column + 1])
           + static_cast<int>(grid.getRow(row)[column + 1]);
  }
  if (column == 0) {// handle 0 case
    return static_cast<int>(grid.getRow(row - 1)[column]) + static_cast<int>(grid.getRow(row + 1)[column + 1])
           + static_cast<int>(grid.getRow(row)[column + 1]) + static_cast<int>(grid.getRow(row + 1)[column])
           + static_cast<int>(grid.getRow(row - 1)[column + 1]);
  }
  if (column == std::size(grid.getRow(row)) - 1) {// Handle size case
    return static_cast<int>(grid.getRow(row - 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
           + static_cast<int>(grid.getRow(row - 1)[column]) + static_cast<int>(grid.getRow(row + 1)[column - 1])
           + static_cast<int>(grid.getRow(row + 1)[column]);
  }
  return static_cast<int>(grid.getRow(row - 1)[column - 1]) + static_cast<int>(grid.getRow(row)[column - 1])
         + static_cast<int>(grid.getRow(row - 1)[column]) + static_cast<int>(grid.getRow(row + 1)[column + 1])
         + static_cast<int>(grid.getRow(row)[column + 1]) + static_cast<int>(grid.getRow(row + 1)[column - 1])
         + static_cast<int>(grid.getRow(row + 1)[column]) + static_cast<int>(grid.getRow(row - 1)[column + 1]);
}

#endif
