#include <iostream>
#include <fstream>
#include <optional>
#include <string>

const int MAX_LENGTH = 255;

enum class mode {
	UNDEFINED_MODE,
	PACK_MODE,
	UNPACK_MODE
};

struct Args
{
	mode currentMode = mode::UNDEFINED_MODE;
	std::string inputFileName;
	std::string outputFileName;
};

mode getCurrentMode(const std::string& modeStr)
{
	mode currentMode;
	if (modeStr == "pack") 
	{ 
		currentMode = mode::PACK_MODE;
	}
	else if (modeStr == "unpack") 
	{ 
		currentMode = mode::UNPACK_MODE;
	}

	return currentMode;
}

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	bool wasError = false;
	if (argc != 4)
	{
		std::cout << "Invalid arguments count.\n"
			<< "Usage: lab_01_04_01.exe pack <input file> <output file> or lab_01_04_01.exe unpack <input file> <output file>.\n";
		return std::nullopt;
	}

	Args args;

	args.currentMode = getCurrentMode(argv[1]);
	if (args.currentMode == mode::UNDEFINED_MODE)
	{
		std::cout << "Invalid mode.\n"
			<< "Usage: pack or unpack.\n";
		return std::nullopt;
	}
	args.inputFileName = argv[2];
	args.outputFileName = argv[3];

	return args;
}

void PrintPackResult(std::ostream& output, const int symbolCount, const char symbol)
{
	output << (char)symbolCount << symbol;
}

bool PackFile(std::istream& input, std::ostream& output)
{
	char currentSymbol;
	char originalSymbol;
	int originalSymbolCount = 0;

	while (input.get(currentSymbol))
	{
		if (originalSymbolCount == 0)
		{
			originalSymbol = currentSymbol;
		}

		if (originalSymbol == currentSymbol)
		{
			originalSymbolCount++;
		}
		
		if (originalSymbol != currentSymbol || originalSymbolCount == MAX_LENGTH)
		{
			PrintPackResult(output, originalSymbolCount, originalSymbol);
			originalSymbol = currentSymbol;
			originalSymbolCount = (originalSymbolCount == MAX_LENGTH) ? 0 : 1;
		}
	}

	if (originalSymbolCount != 0)
	{
		PrintPackResult(output, originalSymbolCount, originalSymbol);
	}

	return true;
}

void PrintUnpackResult(std::ostream& output, const int symbolCount, const char symbol)
{
	for (size_t i = 0; i < symbolCount; i++)
	{
		output << symbol;
	}
}

bool UnpackFile(std::istream& input, std::ostream& output)
{
	char symbolCountCh, symbol;
	int symbolCount;

	while (input.get(symbolCountCh))
	{
		if (input.get(symbol))
		{
			symbolCount = static_cast<unsigned char>(symbolCountCh);
			if (symbolCount == 0)
			{
				return false;
			}
			PrintUnpackResult(output, symbolCount, symbol);
		}
		else
		{
			std::cout << "Damaged input file\n";
			return false;
		}
	}

	return true;
}

bool HandleFileByMode(const mode currentMode, const std::string& inputFileName, std::string& outputFileName)
{
	bool result = true;
	std::ifstream input;
	std::ofstream output;

	input.open(inputFileName, std::ios::binary | std::ios::in);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << inputFileName << "' for reading\n";
		result = false;
	}

	output.open(outputFileName, std::ios::binary | std::ios::out);
	if (!output.is_open())
	{
		std::cout << "Failed to open '" << outputFileName << "' for writing\n";
		result = false;
	}

	switch (currentMode)
	{
	case mode::PACK_MODE:
		result = PackFile(input, output);
		break;
	case mode::UNPACK_MODE:
		result = UnpackFile(input, output);
		break;
	default:
		break;
	}

	if (input.bad())
	{
		std::cout << "Failed to read data from input file\n";
		result = false;
	}

	if (!output.flush()) // ���� �� ������� �������� ������ �� ����
	{
		std::cout << "Failed to write data to output file\n";
		result = false;
	}

	return result;
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);
	if (!args)
	{
		return 1;
	}

	bool result = HandleFileByMode(args->currentMode, args->inputFileName, args->outputFileName);

	return (result) ? 0 : 1;
}