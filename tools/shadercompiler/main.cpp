#include <stdint.h>
#include <iostream>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <vulkan/vulkan.h>
#include <filesystem>
#include <fstream>
#include <vector>


#define ASSERT_ARGS_LENGTH(argc, l, msg) if (l >= argc) { std::cout << msg << std::endl; abort(); }

#define OPT_HELP_IDX 0
#define OPT_INPUT_IDX 1
#define OPT_OUTPUT_IDX 2

#define OPT_VERTEX_STAGE 3
#define OPT_FRAGMENT_STAGE 4

struct Config {
    uint32_t stage{ 0 };
    const char* inputFilePath{ nullptr };
    const char* outputFilePath{ nullptr };
};

struct Options{
    const char* opt{};
    const char* desc{};
};

static const Options opts[] {
        {"-h", "Print help message."},
        {"-i", "Input shader file path."},
        {"-o", "Output compiled shader file path."},
        {"-v", "Compile as a vertex shader."},
        {"-f", "Compile as a fragment shader."},
};
static const uint32_t optionCount = sizeof(opts) / sizeof(Options);

uint32_t GetArgIdx(char* arg) {
    for (uint32_t i = 0; i < optionCount; ++i) {
        if (strcmp(opts[i].opt, arg) == 0) {
            return i;
        }
    }
    return optionCount;
}

void PrintHelpMsg() {
    for (uint32_t i = 0; i < optionCount; ++i) {
        std::cout << opts[i].opt << " : " << opts[i].desc << std::endl;
    }
}

Config ParseArgs(int argc, char** argv) {
    Config config{};

    int c = 1;
    while (c < argc) {
        uint32_t optIdx = GetArgIdx(argv[c]);
        switch (optIdx) {
            case OPT_HELP_IDX:
                PrintHelpMsg();
                exit(0);
            case OPT_INPUT_IDX:
                ASSERT_ARGS_LENGTH(argc, c + 1, "Missing path with -i argument.");
                config.inputFilePath = argv[c + 1];
                c += 2;
                break;
            case OPT_OUTPUT_IDX:
                ASSERT_ARGS_LENGTH(argc, c + 1, "Missing path with -o argument.");
                config.outputFilePath = argv[c + 1];
                c += 2;
                break;
            case OPT_VERTEX_STAGE:
                config.stage = VK_SHADER_STAGE_VERTEX_BIT;
                ++c;
                break;
            case OPT_FRAGMENT_STAGE:
                config.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                ++c;
                break;
            default:
                std::cout << "Bad argument : " << argv[c] << std::endl;
                exit(0);
        }
    }

    return config;
}

EShLanguage GetGlslangStageFromShaderType(uint32_t stage) {
    switch (stage) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return EShLanguage::EShLangVertex;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return EShLanguage::EShLangFragment;
        default:
            return EShLanguage::EShLangVertex;
    }
}

int main(int argc, char** argv) {
    Config config = ParseArgs(argc, argv);

    if (!config.inputFilePath) {
        std::cout << "Missing input file." << std::endl;
        return 0;
    }

    if (!config.outputFilePath) {
        std::cout << "Missing output file." << std::endl;
        return 0;
    }

    if (!config.stage) {
        std::cout << "Missing shader stage." << std::endl;
        return 0;
    }

    std::filesystem::path inputFilePath{ config.inputFilePath };
    std::filesystem::path outputFilePath{ config.outputFilePath };

    if (!std::filesystem::exists(inputFilePath) || !std::filesystem::is_regular_file(inputFilePath)) {
        std::cout << "Invalid input file." << std::endl;
        return 0;
    }

    std::fstream inputFile{ inputFilePath, std::ios_base::in | std::ios_base::binary };
    if (!inputFile.is_open()) {
        std::cout << "Can't open input file." << std::endl;
        return 0;
    }

    inputFile.ignore( std::numeric_limits<std::streamsize>::max() );
    size_t size = inputFile.gcount();
    inputFile.clear();
    inputFile.seekg(0, std::ios_base::beg);

    std::vector<char> data{};
    data.resize(size + 1);
    inputFile.read(data.data(), data.size());
    inputFile.close();
    data[size] = 0;

    inputFile.close();

    glslang::InitializeProcess();

    const char* shSource = data.data();

    EShLanguage stage = GetGlslangStageFromShaderType(config.stage);

    glslang::TShader shader{ stage };
    shader.setAutoMapBindings(true);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    shader.setStrings(&shSource, 1);

    if (!shader.parse(GetDefaultResources(), 100, ENoProfile, false, false, EShMessages::EShMsgDefault)) {
        std::cout << "Glslang failed to parse shader\n" << shader.getInfoLog() << "\n" << shader.getInfoDebugLog() << std::endl;
        return false;
    }

    glslang::TProgram program{};
    program.addShader(&shader);

    if (!program.link(EShMessages::EShMsgDefault) || !program.mapIO()) {
        std::cout << "Glslang failed to link shader\n" << program.getInfoLog() << "\n" << program.getInfoDebugLog() << std::endl;
        return false;
    }

    std::vector<unsigned int> spirv{};
    glslang::SpvOptions spvOptions{};
    spvOptions.disableOptimizer = false;
    glslang::TIntermediate* intermediate = program.getIntermediate(stage);
    glslang::GlslangToSpv(*intermediate, spirv, &spvOptions);

    std::fstream outputFile{ outputFilePath, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary };
    outputFile.write((char*)&config.stage, sizeof(uint32_t));

    uint32_t codeSize{ (uint32_t)(spirv.size() * sizeof(unsigned int)) };
    outputFile.write((char*)&codeSize, sizeof(uint32_t));
    outputFile.write((char*)spirv.data(), codeSize);

    outputFile.close();
}