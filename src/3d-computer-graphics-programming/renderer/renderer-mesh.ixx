export module renderer:mesh;
import shared;
import util;
import :triangle;

export namespace renderer
{
    struct mesh
    {
        std::vector<util::vector_3f> vertices;
        std::vector<face> faces;
        util::vector_3f rotation;
    };
}