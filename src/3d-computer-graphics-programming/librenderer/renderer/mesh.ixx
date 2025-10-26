module;

#include <stdio.h>

export module librenderer:mesh;
import std;
import :math;
import :util;

export namespace renderer
{
	struct mesh
	{
		constexpr mesh() = default;
		mesh(const std::filesystem::path& p)
		{
			*this = from_file(p);
		}
		std::vector<math::vector_4f> vertices;
		std::vector<math::face> faces;
		math::vector_4f rotation;
		math::vector_4f scale{.x=1,.y=1,.z=1};
		math::vector_4f translation;

		constexpr void additively_scale_by(float s) noexcept
		{
			scale.x += s;
			scale.y += s;
			scale.z += s;
		}

		static auto from_file_2(const std::filesystem::path& p) -> mesh
		{
			std::FILE* file = nullptr;
			fopen_s(&file, p.string().c_str(), "r");
			char line[1024];

			mesh m;

			while (std::fgets(line, 1024, file)) {
				// Vertex information
				if (std::strncmp(line, "v ", 2) == 0) {
					math::vector_4f vertex;
					sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
					m.vertices.push_back(vertex);
				}
				// Face information
				if (std::strncmp(line, "f ", 2) == 0) {
					int vertex_indices[3];
					int texture_indices[3];
					int normal_indices[3];
					sscanf_s(
						line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vertex_indices[0], &texture_indices[0], &normal_indices[0],
						&vertex_indices[1], &texture_indices[1], &normal_indices[1],
						&vertex_indices[2], &texture_indices[2], &normal_indices[2]
					);
					math::face face = {
						.a = vertex_indices[0],
						.b = vertex_indices[1],
						.c = vertex_indices[2],
						.color = 0xFFFFFFFF
					};
					m.faces.push_back(face);
				}
			}

			return m;
		}

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
					returnValue.vertices.push_back({
						.x = splits.at(0),
						.y = splits.at(1),
						.z = splits.at(2)
					});
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

