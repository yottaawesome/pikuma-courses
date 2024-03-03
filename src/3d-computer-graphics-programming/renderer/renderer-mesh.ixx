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
    auto mesh_faces = std::array<face, MeshFaces>
    {
        face{ 1, 2, 3 },
        face{ 1, 3, 4 },
        face{ 4, 3, 5 },
        face{ 4, 5, 6 },
        face{ 6, 5, 7 },
        face{ 6, 7, 8 },
        face{ 8, 7, 2 },
        face{ 8, 2, 1 },
        face{ 2, 7, 5 },
        face{ 2, 5, 3 },
        face{ 6, 8, 1 },
        face{ 6, 1, 4 }
    };
}