export module renderer:triangle;
import util;

export namespace renderer
{
	struct face
	{
		int a = 0;
		int b = 0;
		int c = 0;
	};

	struct triangle
	{
		util::vector_2f points[3];
	};
}