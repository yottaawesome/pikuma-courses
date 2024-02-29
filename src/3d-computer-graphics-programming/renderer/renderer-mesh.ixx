export module renderer:mesh;
import shared;
import util;
import :triangle;

export namespace renderer
{
    constexpr auto MeshVertices = 8;
    auto mesh_vertices = std::array<util::vector_3f, MeshVertices>{
        util::vector_3f{ .x = -1, .y = -1, .z = -1 }, // 1
        util::vector_3f{ .x = -1, .y =  1, .z = -1 }, // 2
        util::vector_3f{ .x =  1, .y =  1, .z = -1 }, // 3
        util::vector_3f{ .x =  1, .y = -1, .z = -1 }, // 4
        util::vector_3f{ .x =  1, .y =  1, .z =  1 }, // 5
        util::vector_3f{ .x =  1, .y = -1, .z =  1 }, // 6
        util::vector_3f{ .x = -1, .y =  1, .z =  1 }, // 7
        util::vector_3f{ .x = -1, .y = -1, .z =  1 } // 8
    };

    constexpr auto MeshFaces = 6 * 2;
}