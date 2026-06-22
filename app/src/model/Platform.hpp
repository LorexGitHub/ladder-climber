#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include <vector>

/// A ladder connecting two platforms at alternating x positions.
struct Ladder {
    sf::FloatRect rect;
    Ladder(float x, float y1, float y2) : rect({x, y1}, {20, y2 - y1}) {}
    sf::FloatRect get_bounds() const { return rect; }
};

/// A horizontal platform divided into 10 segments, some of which may be
/// removed (holes) starting from stage 4.
class Platform {
public:
    Platform(float x, float y, float w);

    sf::FloatRect get_bounds() const;
    const sf::RectangleShape& get_shape() const { return shape; }

    /// Returns whether the given segment (0-9) is solid.
    bool is_solid(int s) const { return s < 10 && segments[s]; }
    /// Marks a segment as solid or not.
    void set_solid(int s, bool v) { if (s >= 0 && s < 10) segments[s] = v; }

private:
    sf::RectangleShape shape;
    std::vector<bool> segments = std::vector<bool>(10, true);
};

#endif
