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
        {
            if (fl.line.starts_with("v"))
                vertexLines.push_back(fl.line);
            else
                faceLines.push_back(fl.line);
        }

        mesh returnValue;
        std::ranges::for_each(
            vertexLines, 
            [&returnValue](const std::string& s) 
            {
                std::vector splits = std::ranges::to<std::vector<std::string>>(std::views::split(s, ' ') | std::views::drop(1));
                if (splits.size() != 3)
                    throw std::runtime_error("Expected ddd to be size 3");
                returnValue.vertices.emplace_back(std::stof(splits.at(0)), std::stof(splits.at(1)), std::stof(splits.at(2)));
            }
        );

        std::ranges::for_each(
            faceLines, 
            [&returnValue](const std::string& s) 
            {
                std::vector<std::string> splits = 
                    std::views::split(s, ' ') 
                    | std::views::drop(1) 
                    | std::ranges::to<std::vector<std::string>>();

                if (splits.size() != 3)
                    throw std::runtime_error("Expected splits to be size 3");

                returnValue.faces.emplace_back(
                    std::stof(std::ranges::to<std::vector<std::string>>(splits.at(0) | std::views::split('/') | std::views::take(1)).at(0)),
                    std::stof(std::ranges::to<std::vector<std::string>>(splits.at(1) | std::views::split('/') | std::views::take(1)).at(0)), 
                    std::stof(std::ranges::to<std::vector<std::string>>(splits.at(2) | std::views::split('/') | std::views::take(1)).at(0))
                );
            }
        );

        return returnValue;
    }
}