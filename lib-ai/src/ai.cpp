#include "ai.hpp"
#include <iostream>
#include <format>
#include <ort_genai_c.h>
#include <ort_genai.h>

std::unique_ptr<OgaModel> AI::model = nullptr;
std::unique_ptr<OgaTokenizer> AI::tokenizer = nullptr;
std::unique_ptr<OgaGeneratorParams> AI::params = nullptr;

void AI::Initialize()
{
    if (model != nullptr)
    {
        std::cout << "Initialized skipped" << std::endl;
        return;
    }
    model = OgaModel::Create("/data/data/com.example.quickmaths/files/model");
    tokenizer = OgaTokenizer::Create(*model);
    params = OgaGeneratorParams::Create(*model);
    params->SetSearchOption("max_length", 250);
    std::cout << "Initialized model" << std::endl;
}

std::string AI::Generate(std::string input)
{
    auto sequences = OgaSequences::Create();
    input = std::format(R"(
        Convert the following text into a computer-readable mathematical equation.
        Output only the equation, without any explanation.
        Input: What is the % increase of 5 to 10
        Output: (10 - 5) / 5 * 100
        Input: 5 ounces to pounds
        Output: 5 / 16
        Input: 20$ for 5 ounces in $/lb
        Output: 20 / (5 / 16)
        Input: 24 pounds for 36$ in $/lb
        Output: 36 / 24
        Input: 5 lb to kg
        Out)",
                        input);
    input = std::format("<|user|>{}\n <|end|>\n<|assistant|>", input);
    tokenizer->Encode(input.c_str(), *sequences);
    auto generator = OgaGenerator::Create(*model, *params);
    generator->AppendTokenSequences(*sequences);
    while (!generator->IsDone())
    {
        generator->GenerateNextToken();
    }
    auto output_sequence = generator->GetSequenceData(0);
    auto output_string = tokenizer->Decode(output_sequence, generator->GetSequenceCount(0));
    return GetOutput(std::string(output_string.p_));
}

std::string AI::Trim(const std::string &str)
{
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : "";
}

std::string AI::GetOutput(const std::string &input)
{
    const std::string marker = "Output: ";
    size_t pos = input.rfind(marker);
    if (pos != std::string::npos)
    {
        return Trim(input.substr(pos + marker.length()));
    }
    return "";
}

void AI::Shutdown()
{
    params.reset();
    tokenizer.reset();
    model.reset();
    OgaShutdown();
}