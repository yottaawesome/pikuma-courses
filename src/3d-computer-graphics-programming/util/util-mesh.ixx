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

    // adapted from https://mobiarch.wordpress.com/2023/12/17/reading-a-file-line-by-line-using-c-ranges/
    struct file_line
    {
        std::string line;
        size_t line_number = 0;
 
        friend std::istream &operator>>(std::istream &s, file_line& fl)
        {
            std::getline(s, fl.line);
 
            ++fl.line_number;
 
            return s;
        }
    };

    [[nodiscard("Loading a mesh and immediately discarding it is pointless.")]]
    mesh load_obj_file(const std::filesystem::path p)
    {
        if (not std::filesystem::exists(p))
            throw std::runtime_error("Path not found");

        std::ifstream file;
        file.open(p.string());
        if (file.fail())
            throw std::runtime_error("Stream in bad state");

        std::vector<std::string> vertexLines;
        std::vector<std::string> faceLines;
        auto filter =
            std::views::istream<file_line>(file)
            | std::views::filter([](file_line& s) { return s.line.starts_with("v ") or s.line.starts_with("f "); });
        for (const auto& fl : filter)
            if (fl.line.starts_with("v"))
                vertexLines.push_back(fl.line);
            else
                faceLines.push_back(fl.line);

        mesh returnValue;
        for (std::string_view s : vertexLines)
        {
            std::vector splits = 
                std::views::split(s, ' ') 
                | std::views::drop(1) // drop 'v' prefix
                | std::ranges::to<std::vector<std::string>>()
                | std::views::transform([](const std::string& sv) { return std::stof(sv); })
                | std::ranges::to<std::vector<float>>();
            returnValue.vertices.emplace_back(splits.at(0), splits.at(1), splits.at(2));
        }

        for (std::string_view s : faceLines)
        {
            //auto getFace = std::views::split('/') | std::views::take(1) | std::views::join | std::ranges::to<std::string>();
            std::vector<int> faceCoords =
                std::views::split(s, ' ')
                | std::views::drop(1) // drop 'f' prefix
                | std::ranges::to<std::vector<std::string>>()
                | std::views::take(3) // take first 3 triplets of x/y/z
                | std::views::transform( // convert first element of x/y/z triplet to int
                    [](std::string_view sv) 
                    {
                        return std::stoi(sv | std::views::split('/') | std::views::take(1) | std::views::join | std::ranges::to<std::string>()); 
                    }) 
                | std::ranges::to<std::vector<int>>();

            returnValue.faces.emplace_back(faceCoords.at(0), faceCoords.at(1), faceCoords.at(2));
        }

        return returnValue;
    }
}