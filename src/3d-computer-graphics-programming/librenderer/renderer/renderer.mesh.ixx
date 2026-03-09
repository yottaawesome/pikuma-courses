module;

#include <stdio.h>

export module librenderer:renderer.mesh;
import std;
import :math;
import :util;
import :renderer.primitives;

namespace
{
	constexpr auto total_cube_vertices = 8;
	constexpr auto total_cube_faces = 6 * 2;

	constexpr auto cube_vertices = std::array
	{
		math::vector_4f{.x = -1, .y = -1, .z = -1 }, // 1
		math::vector_4f{.x = -1, .y = 1, .z = -1 }, // 2
		math::vector_4f{.x = 1, .y = 1, .z = -1 }, // 3
		math::vector_4f{.x = 1, .y = -1, .z = -1 }, // 4
		math::vector_4f{.x = 1, .y = 1, .z = 1 }, // 5
		math::vector_4f{.x = 1, .y = -1, .z = 1 }, // 6
		math::vector_4f{.x = -1, .y = 1, .z = 1 }, // 7
		math::vector_4f{.x = -1, .y = -1, .z = 1 } // 8
	};

	constexpr auto cube_faces = std::array{
		// front
		renderer::face{.a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
		// right
		renderer::face{.a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
		// back
		renderer::face{.a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
		// left
		renderer::face{.a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
		// top
		renderer::face{.a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }},
		// bottom
		renderer::face{.a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }},
		renderer::face{.a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }}
	};

	template<typename T, size_t N>
	auto vector_from_array(const std::array<T, N>& array) -> std::vector<T>
	{
		return std::vector<T>{
			std::make_move_iterator(std::begin(array)),
			std::make_move_iterator(std::end(array))
		};
	}
}

export namespace renderer
{
	struct mesh
	{
		constexpr mesh() = default;
		mesh(
			const std::vector<math::vector_4f>& vertices,
			const std::vector<renderer::face>& faces
		) : vertices(vertices), faces(faces) 
		{ }

		mesh(const std::filesystem::path& p)
		{
			*this = from_file(p);
		}
		std::vector<math::vector_4f> vertices;
		std::vector<renderer::face> faces;
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
					renderer::face face = {
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

		struct face_triplet
		{
			int vertex_index;
			int uv_index;
			int normal_index;
		};

		[[nodiscard("Loading a mesh and immediately discarding it is pointless.")]]
		static auto from_file(const std::filesystem::path& p) -> mesh
		{
			if (not std::filesystem::exists(p))
				throw std::runtime_error("Path not found");

			std::ifstream file;
			file.open(p.string());
			if (file.fail())
				throw std::runtime_error("Stream in bad state");

			auto filter = std::views::istream<util::file_line>(file)
				| std::views::filter(
					[](util::file_line& s) static -> bool
					{ 
						return s.line.starts_with("v ") 
							or s.line.starts_with("vt ")
							or s.line.starts_with("f "); 
					});

			constexpr auto colors = std::array{ 0xffff0000, 0xff00ff00, 0xff0000ff };

			auto returnValue = mesh{};
			auto texcoords = std::vector<tex2_coordinates>{};
			for (int i = 0; const auto& fl : filter)
			{
				if (fl.line.starts_with("v "))
				{
					auto splits = std::vector{
						std::views::split(fl.line, ' ')
						| std::views::drop(1) // drop 'v' prefix
						| std::ranges::to<std::vector<std::string>>()
						| std::views::transform(
							[](const std::string& sv) static -> float { return std::stof(sv); })
						| std::ranges::to<std::vector<float>>()
					};
					returnValue.vertices.push_back({
						.x = splits.at(0),
						.y = splits.at(1),
						.z = splits.at(2)
					});
				}
				else if (fl.line.starts_with("vt "))
				{
					auto splits =
						std::views::split(fl.line, ' ')
						| std::views::drop(1) // drop 'vt' prefix
						| std::ranges::to<std::vector<std::string>>() // split into vector of strings
						| std::views::transform( // convert each string to float
							[](const std::string& sv) static -> float { return std::stof(sv); })
						| std::ranges::to<std::vector<float>>(); // convert to vector of floats
					texcoords.emplace_back(splits.at(0), splits.at(1));
				}
				else if (fl.line.starts_with("f"))
				{
					//auto getFace = std::views::split('/') | std::views::take(1) | std::views::join | std::ranges::to<std::string>();
					auto faceCoords = std::vector{
						std::views::split(fl.line, ' ')
						| std::views::drop(1) // drop 'f' prefix
						| std::ranges::to<std::vector<std::string>>()
						| std::views::take(3) // take first 3 triplets of x/y/z
						| std::views::transform( // convert first element of x/y/z triplet to int
							[](const std::string& sv) -> face_triplet
							{
								auto parts = sv
									| std::views::split('/')
									| std::ranges::to<std::vector<std::string>>()
									| std::views::transform(
										[](std::string_view inner_sv) static -> int
										{
											int value;
											auto [ptr, ec] = std::from_chars(inner_sv.data(), inner_sv.data() + inner_sv.size(), value);
											if (ec != std::errc{})
												throw std::runtime_error("Failed to parse face triplet");
											return value;
										})
									| std::ranges::to<std::vector>();
								return {
									.vertex_index = parts.at(0),
									.uv_index = parts.at(1),
									.normal_index = parts.at(2)
								};
							})
						| std::ranges::to<std::vector>()
					};

					// For every two faces, increment the colour index
					auto color_index = i % colors.size();
					returnValue.faces.emplace_back(
						faceCoords.at(0).vertex_index - 1, 
						faceCoords.at(1).vertex_index - 1, 
						faceCoords.at(2).vertex_index - 1, 
						colors[color_index],
						texcoords.at(faceCoords.at(0).uv_index - 1), // OBJ indices are 1-based
						texcoords.at(faceCoords.at(1).uv_index - 1),
						texcoords.at(faceCoords.at(2).uv_index - 1)
					);
					if (returnValue.faces.size() % 2 == 0)
						i++;
				}
			}
			return returnValue;
		}
	};

	auto load_cube_mesh() -> mesh
	{
		return mesh(vector_from_array(cube_vertices), vector_from_array(cube_faces));
	}
}

