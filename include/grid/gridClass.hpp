
#include <vector>
class Grid
{
  std::vector<std::vector<bool>> grid;

public:
  explicit Grid(auto... Args) : grid(std::move(Args)...) {}// Nice constructor that forwards to all vector constructors
  [[nodiscard]] auto getRow(std::size_t index) const -> std::vector<bool> const & { return grid[index]; }
  [[nodiscard]] auto getColumn(std::size_t index) const -> std::vector<bool>
  {
    std::vector<bool> col(std::size(grid));
    for (const auto &row : grid) { col.push_back(row[index]); }
    return col;
  }
  [[nodiscard]] auto numOfRows() const -> std::size_t { return std::size(grid); }
  [[nodiscard]] auto numOfColumns() const -> std::size_t { return std::size(grid[0]); }
};
