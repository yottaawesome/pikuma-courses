export module librenderer:mesh;
import std;
import :math;
import :util;

export namespace renderer
{
    struct mesh
    {
        mesh() = default;
        mesh(const std::filesystem::path& p)
        {
			*this = from_file(p);
        }
        std::vector<math::vector_3f> vertices;
        std::vector<math::face> faces;
        math::vector_3f rotation;

        [[nodiscard("Loading a mesh and immediately discarding it is pointless.")]]
        static auto from_file(const std::filesystem::path& p) -> mesh
        {
            if (not std::filesystem::exists(p))
                throw std::runtime_error("Path not found");

            std::ifstream file;
            file.open(p.string());
            if (file.fail())
                throw std::runtime_error("Stream in bad state");

            auto filter =
                std::views::istream<util::file_line>(file)
                | std::views::filter([](util::file_line& s) { return s.line.starts_with("v ") or s.line.starts_with("f "); });

            constexpr std::array colors{ 0xffff0000, 0xff00ff00, 0xff0000ff };

            mesh returnValue;
            for (int i = 0; const auto& fl : filter)
            {
                if (fl.line.starts_with("v"))
                {
                    std::vector splits =
                        std::views::split(fl.line, ' ')
                        | std::views::drop(1) // drop 'v' prefix
                        | std::ranges::to<std::vector<std::string>>()
                        | std::views::transform(
                            [](const std::string& sv) -> float { return std::stof(sv); })
                        | std::ranges::to<std::vector<float>>();
                    returnValue.vertices.emplace_back(splits.at(0), splits.at(1), splits.at(2));
                }
                else
                {
                    //auto getFace = std::views::split('/') | std::views::take(1) | std::views::join | std::ranges::to<std::string>();
                    std::vector faceCoords =
                        std::views::split(fl.line, ' ')
                        | std::views::drop(1) // drop 'f' prefix
                        | std::ranges::to<std::vector<std::string>>()
                        | std::views::take(3) // take first 3 triplets of x/y/z
                        | std::views::transform( // convert first element of x/y/z triplet to int
                            [](std::string_view sv) -> int
                            {
                                return std::stoi(
                                    sv
                                    | std::views::split('/')
                                    | std::views::take(1)
                                    | std::views::join
                                    | std::ranges::to<std::string>()
                                );
                            })
                        | std::ranges::to<std::vector<int>>();

                    // For every two faces, increment the colour index
                    auto color_index = i % colors.size();
                    returnValue.faces.emplace_back(faceCoords.at(0), faceCoords.at(1), faceCoords.at(2), colors[color_index]);
                    if (returnValue.faces.size() % 2 == 0)
                        i++;
                }
            }

            return returnValue;
		}
    };
}

