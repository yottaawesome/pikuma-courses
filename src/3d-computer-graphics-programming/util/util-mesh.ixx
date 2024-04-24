export module util:mesh;
import shared;
import :primitives;

export namespace util
{
    struct mesh
    {
        std::vector<util::vector_3f> vertices;
        std::vector<face> faces;
        util::vector_3f rotation;
    };
}