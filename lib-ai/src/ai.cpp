#include "ai.hpp"
#include <iostream>
#include <format>
#include "llama.h"

llama_model *AI::model = nullptr;
const llama_vocab *AI::vocab;

void AI::Initialize()
{
    // std::string model_path = "/data/data/com.example.quickmaths/files/model.gguf";
    std::string model_path = "AceMath-1.5B-Instruct.i1-Q4_K_M.gguf";
    ggml_backend_load_all();
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = ngl;
    AI::model = llama_model_load_from_file(model_path.c_str(), model_params);
    AI::vocab = llama_model_get_vocab(model);
}

std::string AI::Generate(std::string prompt)
{
    prompt = systemMessage + prompt;
    std::cout << prompt << std::endl;
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 4096;
    ctx_params.n_batch = 128;
    llama_context *ctx = llama_init_from_model(model, ctx_params);
    const char *tmpl = llama_model_chat_template(model, nullptr);
    std::vector<llama_chat_message> messages;
    std::vector<char> formatted(llama_n_ctx(ctx));
    messages.push_back({"user", strdup(prompt.c_str())});
    auto new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    prompt = std::string(formatted.begin(), formatted.begin() + new_len);
    const int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), NULL, 0, true, true);
    std::vector<llama_token> prompt_tokens(n_prompt_tokens);
    llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), true, true);
    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
    auto sparams = llama_sampler_chain_default_params();
    llama_sampler *smpl = llama_sampler_chain_init(sparams);
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());
    int n_decode = 0;
    llama_token new_token_id;
    std::string final;
    for (int n_pos = 0; n_pos + batch.n_tokens < n_prompt_tokens + n_predict;)
    {
        llama_decode(ctx, batch);
        n_pos += batch.n_tokens;
        {
            new_token_id = llama_sampler_sample(smpl, ctx, -1);
            if (llama_vocab_is_eog(vocab, new_token_id))
            {
                break;
            }
            char buf[128];
            int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
            final += std::string(buf, n);
            batch = llama_batch_get_one(&new_token_id, 1);
            n_decode += 1;
        }
    }
    llama_sampler_free(smpl);
    llama_free(ctx);
    return final;
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
    llama_model_free(model);
}