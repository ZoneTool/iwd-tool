#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>

std::filesystem::path dump_folder = "";

bool folder_has_file(const std::filesystem::path& folder, const std::filesystem::path& path)
{
	if (!std::filesystem::exists(folder / path.filename()))
	{
		return false;
	}

	return true;
}
bool iw4_has_image(const std::filesystem::path& path)
{
	return folder_has_file("iw4_images", path.filename());
}
bool iw4_has_sound(const std::filesystem::path& path)
{
	return folder_has_file("iw4_sound", path.filename());
}

void copy_codo_image(const std::filesystem::path& path)
{
	if (!iw4_has_image(path) && !folder_has_file(dump_folder / "images", path.filename()))
	{
		if (std::filesystem::exists(path))
		{
			std::filesystem::copy_file(path, dump_folder / "images" / path.filename());
		}
		else
		{
			std::cout << "could not find file: " << path << std::endl;
		}
	}
}
void copy_codo_sound(const std::filesystem::path& path)
{
	if (!iw4_has_sound(path) && !folder_has_file(dump_folder / "sound", path.filename()))
	{
		std::filesystem::create_directories(dump_folder / "sound" / path.parent_path());
		std::filesystem::copy_file(path, dump_folder / "sound" / path.filename());
	}
}

std::vector < std::string > split(const std::string& raw_input, const std::vector < char >& delims)
{
	std::vector < std::string > strings;

	auto find_first_delim = [](const std::string& input, const std::vector < char >& delims) -> std::pair < char, std::size_t>
	{
		auto first_delim = 0;
		auto first_delim_index = static_cast<std::size_t>(-1);
		auto index = 0u;

		for (auto& delim : delims)
		{
			if ((index = input.find(delim)) != std::string::npos)
			{
				if (first_delim_index == -1 || index < first_delim_index)
				{
					first_delim = delim;
					first_delim_index = index;
				}
			}
		}

		return { first_delim, first_delim_index };
	};

	auto input = raw_input;

	while (!input.empty())
	{
		const auto split_delim = find_first_delim(input, delims);
		if (split_delim.first != 0)
		{
			strings.push_back(input.substr(0, split_delim.second));
			input = input.substr(split_delim.second + 1);
		}
		else
		{
			break;
		}
	}

	strings.push_back(input);
	return strings;
}

std::vector < std::string > split(const std::string& str, char delimiter)
{
	return split(str, std::vector < char >({ delimiter }));
}

std::string read_file(const std::filesystem::path& path)
{
	std::string buffer;

	if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
	{
		const auto file_size = std::filesystem::file_size(path);
		if (file_size > 0)
		{
			buffer.resize(file_size);

			const auto fp = fopen(path.string().data(), "rb");
			if (fp)
			{
				fread(&buffer[0], file_size, 1, fp);
				fclose(fp);
			}
		}
	}

	return buffer;
}

int main(int argc, char** argv)
{
	std::cout << "iwd-tool by RektInator." << std::endl;

	try
	{
		auto dump_file = static_cast<std::string>("dump.cfg");
		
		if (argc == 2)
		{
			dump_file = argv[1];
			dump_folder = std::filesystem::path(dump_file).replace_extension("").filename();

			std::filesystem::create_directories(dump_folder / "images");
			std::filesystem::create_directories(dump_folder / "sound");
		}
		
		std::filesystem::create_directory("images");
		std::filesystem::create_directory("sound");
		
		std::cout << "collecting images..." << std::endl;

		const auto dump_buffer = read_file(dump_file);
		if (dump_buffer.empty())
		{
			std::cout << dump_file << " is empty or missing." << std::endl;
			return 1;
		}

		auto dump_lines = split(dump_buffer, { '\r', '\n' });
		for (auto& line : dump_lines)
		{
			auto texture_data = split(line, ' ');
			if (texture_data.size() >= 2)
			{
				auto current_file = std::filesystem::path(texture_data[1]).filename();
				if (current_file.extension() == ".iwi")
				{
					auto texture_path = "codo_images" / std::filesystem::path(texture_data[1]).filename();
					copy_codo_image(texture_path);
				}
				else if (current_file.extension() == ".mp3" || current_file.extension() == ".wav")
				{
					auto sound_without_root = &texture_data[1][6];
					auto sound_path = "codo_sound" / std::filesystem::path(sound_without_root);
					copy_codo_sound(sound_path);
				}
			}
		}

		std::cout << "done." << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << "an exception occured: " << ex.what() << std::endl;
	}

	return 0;
}
