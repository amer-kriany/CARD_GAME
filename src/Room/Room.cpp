#include "Room.h"

#include <vector>

#include "WorldConfig.h"

#ifndef ASSET_DIR
#define ASSET_DIR "./"
#endif

namespace
{
    // Room size. The table is about 4.5 x 3.5, so 10 x 10 leaves space
    // for the three decor pieces.
    constexpr float HALF = world::ROOM_HALF;
    constexpr float CEILING_Y = world::CEILING_Y;
    constexpr float FLOOR_Y = world::FLOOR_Y;

    // basic.frag material 5 = plain texture x tint, without the rounded-corner
    // mask (that mask would cut gaps between walls and floor).
    constexpr int PLAIN_TEXTURE = 5;

    // How many times the texture repeats over a surface (about 2.5 units each).
    constexpr float TILE_FLOOR = 4.0f;
    constexpr float TILE_WALL_U = 4.0f;
    constexpr float TILE_WALL_V = 2.2f;

    std::vector<glm::vec2> tiledUV(float u, float v)
    {
        return {
            {0.0f, 0.0f},
            {u,    0.0f},
            {u,    v},
            {0.0f, v}
        };
    }

    // ---- Waseem's torn poster ----
    // Each row: x, y, z, u, v. The corner is eaten away along a jagged edge
    // at the bottom right. Original numbers are unchanged.
    constexpr float POSTER_POINTS[23][5] = {
        {-1.50f, 0.50f, 4.980f, 0.000f, 0.000f},
        { 0.30f, 0.50f, 4.980f, 0.600f, 0.000f},
        { 0.36f, 0.57f, 4.976f, 0.620f, 0.035f},
        { 0.44f, 0.55f, 4.974f, 0.647f, 0.025f},
        { 0.50f, 0.65f, 4.970f, 0.667f, 0.075f},
        { 0.58f, 0.63f, 4.968f, 0.693f, 0.065f},
        { 0.63f, 0.75f, 4.964f, 0.710f, 0.125f},
        { 0.72f, 0.73f, 4.960f, 0.740f, 0.115f},
        { 0.76f, 0.86f, 4.956f, 0.753f, 0.180f},
        { 0.85f, 0.84f, 4.955f, 0.783f, 0.170f},
        { 0.88f, 0.96f, 4.958f, 0.793f, 0.230f},
        { 0.98f, 0.94f, 4.960f, 0.827f, 0.220f},
        { 1.03f, 1.05f, 4.963f, 0.843f, 0.275f},
        { 1.12f, 1.04f, 4.965f, 0.873f, 0.270f},
        { 1.16f, 1.16f, 4.968f, 0.887f, 0.330f},
        { 1.25f, 1.15f, 4.970f, 0.917f, 0.325f},
        { 1.29f, 1.27f, 4.973f, 0.930f, 0.385f},
        { 1.38f, 1.26f, 4.975f, 0.960f, 0.380f},
        { 1.41f, 1.38f, 4.977f, 0.970f, 0.440f},
        { 1.47f, 1.40f, 4.978f, 0.990f, 0.450f},
        { 1.50f, 1.50f, 4.980f, 1.000f, 0.500f},
        { 1.50f, 2.50f, 4.980f, 1.000f, 1.000f},
        {-1.50f, 2.50f, 4.980f, 0.000f, 1.000f}
    };

    // The poster image is portrait (640 x 1136) but Waseem's rectangle is
    // 3 wide x 2 tall, which stretches it sideways. Squeeze x so a 2-tall
    // poster is 640/1136 * 2 wide.
    // Where the poster hangs along the back wall (0 = middle). It is moved
    // to the left so the character, who stands in the middle, never hides it.
    constexpr float POSTER_X = -2.6f;

    constexpr float POSTER_X_SCALE = (640.0f / 1136.0f * 2.0f) / 3.0f;

    std::vector<glm::vec3> posterPositions()
    {
        std::vector<glm::vec3> positions;

        // His floor was at y = -1, ours is at FLOOR_Y: keep the same
        // height above the floor.
        // POSTER_DROP lowers it a bit more so the top is never cut off
        // by the top of the screen in camera 2.
        const float POSTER_DROP = 0.25f;
        const float lift = FLOOR_Y - (-1.0f) - POSTER_DROP;

        for (const auto& p : POSTER_POINTS)
        {
            // z is mirrored (4.98 -> -4.98): his poster was on the wall
            // behind camera 2. This puts it on the BACK wall, which
            // camera 2 looks at.
            positions.emplace_back(
                p[0] * POSTER_X_SCALE + POSTER_X, p[1] + lift, -p[2]);
        }

        return positions;
    }

    std::vector<glm::vec2> posterUV()
    {
        std::vector<glm::vec2> uv;

        for (const auto& p : POSTER_POINTS)
        {
            uv.emplace_back(p[3], p[4]);
        }

        return uv;
    }
}

Room::Room()
    : floorTexture_(ASSET_DIR "textures/wood6.jpg"),
      wallTexture_(ASSET_DIR "textures/wood7.jpg"),
      posterTexture_(ASSET_DIR "textures/g.jpg"),

      floor_(
          {{-HALF, FLOOR_Y, -HALF}, {HALF, FLOOR_Y, -HALF},
           {HALF, FLOOR_Y,  HALF},  {-HALF, FLOOR_Y,  HALF}},
          tiledUV(TILE_FLOOR, TILE_FLOOR)),

      backWall_(
          {{-HALF, FLOOR_Y, -HALF}, {HALF, FLOOR_Y, -HALF},
           {HALF, CEILING_Y, -HALF}, {-HALF, CEILING_Y, -HALF}},
          tiledUV(TILE_WALL_U, TILE_WALL_V)),

      leftWall_(
          {{-HALF, FLOOR_Y, -HALF}, {-HALF, FLOOR_Y, HALF},
           {-HALF, CEILING_Y, HALF}, {-HALF, CEILING_Y, -HALF}},
          tiledUV(TILE_WALL_U, TILE_WALL_V)),

      rightWall_(
          {{HALF, FLOOR_Y, -HALF}, {HALF, FLOOR_Y, HALF},
           {HALF, CEILING_Y, HALF}, {HALF, CEILING_Y, -HALF}},
          tiledUV(TILE_WALL_U, TILE_WALL_V)),

      frontWall_(
          {{-HALF, FLOOR_Y, HALF}, {HALF, FLOOR_Y, HALF},
           {HALF, CEILING_Y, HALF}, {-HALF, CEILING_Y, HALF}},
          tiledUV(TILE_WALL_U, TILE_WALL_V)),

      ceiling_(
          {{-HALF, CEILING_Y, -HALF}, {HALF, CEILING_Y, -HALF},
           {HALF, CEILING_Y,  HALF},  {-HALF, CEILING_Y,  HALF}},
          tiledUV(TILE_FLOOR, TILE_FLOOR)),

      poster_(posterPositions(), posterUV())
{
}

void Room::draw(const Shader& shader, bool drawCeiling) const
{
    shader.use();

    const glm::mat4 identity(1.0f);

    floor_.draw(shader, floorTexture_, identity, PLAIN_TEXTURE);

    backWall_.draw(shader, wallTexture_, identity, PLAIN_TEXTURE);
    leftWall_.draw(shader, wallTexture_, identity, PLAIN_TEXTURE);
    rightWall_.draw(shader, wallTexture_, identity, PLAIN_TEXTURE);
    frontWall_.draw(shader, wallTexture_, identity, PLAIN_TEXTURE);

    if (drawCeiling)
    {
        ceiling_.draw(shader, wallTexture_, identity, PLAIN_TEXTURE);
    }

    // Poster last: it floats 0.02 in front of the back wall.
    poster_.draw(shader, posterTexture_, identity, PLAIN_TEXTURE);
}