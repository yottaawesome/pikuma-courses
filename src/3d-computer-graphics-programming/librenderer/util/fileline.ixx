export module librenderer:util.fileline;
import std;

export namespace util
{
    // adapted from https://mobiarch.wordpress.com/2023/12/17/reading-a-file-line-by-line-using-c-ranges/
    struct file_line
    {
        std::string line;
        size_t line_number = 0;
 
        friend auto operator>>(std::istream &s, file_line& fl) -> std::istream&
        {
            std::getline(s, fl.line);
            ++fl.line_number;
            return s;
        }
    };
}
