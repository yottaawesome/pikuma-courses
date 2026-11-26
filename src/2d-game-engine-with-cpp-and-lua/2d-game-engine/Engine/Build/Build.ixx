export module Engine:Build;

export namespace Engine
{
	constexpr auto IsDebugBuild =
#ifdef _DEBUG
		true;
#else
		false;
#endif
	constexpr auto IsReleaseBuild = not IsDebugBuild;
}
