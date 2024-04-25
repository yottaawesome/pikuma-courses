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

        mesh returnValue;
        std::vector<std::string> vertexLines;
        std::vector<std::string> faceLines;
        for (
            const auto& fl : 
                std::views::istream<file_line>(file) 
                    | std::views::filter([](file_line& s) { return s.line.starts_with("v ") or s.line.starts_with("f "); })
        )
        {
            if (fl.line.starts_with("v"))
                vertexLines.push_back(fl.line);
            else
                faceLines.push_back(fl.line);
        }

        std::ranges::for_each(
            vertexLines, 
            [&returnValue](const std::string& s) 
            {
                std::vector ddd = std::ranges::to<std::vector<std::string>>(std::views::split(s, ' ') | std::views::drop(1));
                returnValue.vertices.emplace_back(std::stof(ddd.at(0)), std::stof(ddd.at(1)), std::stof(ddd.at(2)));
            }
        );

        std::ranges::for_each(
            faceLines, 
            [&returnValue](const std::string& s) 
            {
                std::vector<std::string> ddd = 
                    std::views::split(s, ' ') 
                    | std::views::drop(1) 
                    | std::ranges::to<std::vector<std::string>>();

                returnValue.faces.emplace_back(
                    std::stof(std::ranges::to<std::vector<std::string>>(ddd.at(0) | std::views::split('/') | std::views::take(1)).at(0)),
                    std::stof(std::ranges::to<std::vector<std::string>>(ddd.at(1) | std::views::split('/') | std::views::take(1)).at(0)), 
                    std::stof(std::ranges::to<std::vector<std::string>>(ddd.at(2) | std::views::split('/') | std::views::take(1)).at(0))
                );
            }
        );

        return returnValue;
    }
}