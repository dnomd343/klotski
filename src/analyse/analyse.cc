#include <queue>
#include "core.h"
#include "analyse.h"

#include <list>
#include <iostream>
#include "raw_code.h"
#include "common.h"

Core Analyse::new_core() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

void dump_python_case(uint64_t raw_code) {
    std::string result;
    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
        uint32_t x = addr % 4;
        uint32_t y = (addr - x) / 4;

        char block[11]; // eg: `[0,0,1,1]`

        switch (raw_code & 0b111) {
            case B_1x1:
                sprintf(block, "[%d,%d,1,1],", x, y);
                result += block;
                break;
            case B_1x2:
                sprintf(block, "[%d,%d,1,2],", x, y);
                result += block;
                break;
            case B_2x1:
                sprintf(block, "[%d,%d,2,1],", x, y);
                result += block;
                break;
            case B_2x2:
                sprintf(block, "[%d,%d,2,2],", x, y);
                result += block;
                break;
            default:
                continue;
        }
    }
    result[result.length() - 1] = '\0'; // remove last `,`
    std::cout << '[' << result.c_str() << ']';
}

void Analyse::backtrack(const std::vector<uint64_t> &raw_code_list) {

    // backtrack start at code
//    std::cout << "start backtrack" << std::endl;

    std::queue<analyse_t*> track_cache;

    std::vector<std::vector<backtrack_t*> > layer_data;

    std::unordered_map<uint64_t, backtrack_t> track_data;

    // TODO: confirm that code exist

    /// layer init
//    auto max_step = cases[code].step; // TODO: update max step cal
//    auto max_step = 81;

    uint32_t max_step = 0;
    for (const auto &code : raw_code_list) {
        // TODO: check whether cases include code
        if (cases[code].step > max_step) {
            max_step = cases[code].step;
        }
    }

    layer_data.resize(max_step + 1);

    /// init track begin cases
//    {
//        auto layer_num = cases[code].step;
//
//        track_cache.emplace(&cases[code]);
//
//        auto ptr = track_data.emplace(code, backtrack_t {
//            .code = code,
//            .layer_num = layer_num,
//            .layer_index = (uint32_t)layer_data[layer_num].size(),
//        });
//
//        layer_data[layer_num].emplace_back(&ptr.first->second);
//    }

    for (auto code : raw_code_list) {

        auto layer_num = cases[code].step;

        track_cache.emplace(&cases[code]);

        auto ptr = track_data.emplace(code, backtrack_t {
            .code = code,
            .layer_num = layer_num,
            .layer_index = (uint32_t)layer_data[layer_num].size(),
        });

        layer_data[layer_num].emplace_back(&ptr.first->second);
    }

//    for (const auto &t : track_data) {
//        std::cout << RawCode(t.second.code).dump_case() << std::endl;
//        std::cout << t.second.layer_num << " " << t.second.layer_index << std::endl;
//    }

    while (!track_cache.front()->src.empty()) {

        auto current = track_cache.front();

        for (auto src : current->src) {

            auto find_ret = track_data.find(src->code);

            if (find_ret != track_data.end()) { // already exist

                find_ret->second.next.emplace_back(
                    &track_data[current->code]
                );

            } else { // insert new case

                track_cache.emplace(src);

                auto ptr = track_data.emplace(src->code, backtrack_t {
                    .code = src->code,
                    .layer_num = src->step,
                    .layer_index = (uint32_t)layer_data[src->step].size(),
                    .next = std::list<backtrack_t*>{&track_data[current->code]},
                });

                layer_data[src->step].emplace_back(&ptr.first->second);

            }

        }
        track_cache.pop();

    }

    backtrack_t *root = &track_data[track_cache.front()->code];

//    std::cout << "Size = " << track_data.size() << std::endl;
//    std::cout << "Root" << std::endl;
//    std::cout << RawCode(root->code).dump_case() << std::endl;

//    for (auto layer : layer_data) {
//        std::cout << "-----------------------" << std::endl;
//        std::cout << "layer size = " << layer.size() << std::endl;
//
//        for (auto element : layer) {
//            std::cout << "(" << element->layer_num << ", " << element->layer_index << ") -> ";
//            for (auto next : element->next) {
//                std::cout << "(" << next->layer_num << ", " << next->layer_index << ") ";
//            }
//            std::cout << std::endl;
//
//            std::cout << RawCode(element->code).dump_case() << std::endl;
//        }
//    }


    printf("layer:\n");
    for (uint32_t num = 0; num < layer_data.size(); ++num) {
        auto layer = &layer_data[num];
        printf("- [");
        for (auto element : *layer) {
//            printf(&",\"%015lX\""[element == (*layer)[0]], element->code);
            printf("%s", &","[element == (*layer)[0]]);
            dump_python_case(element->code);
        }
        printf("] # layer %d\n", num);
    }
    printf("\n");

    printf("next:\n");
    for (uint32_t num = 0; num + 1 < layer_data.size(); ++num) {
        auto layer = &layer_data[num];
        printf("- ");
        for (uint32_t index = 0; index < layer->size(); ++index) {
            auto element = &(*layer)[index];
            printf("%s", &"  - ["[!index * 2]);
            bool first_flag = true;
            for (auto next : (*element)->next) {
                printf(&",%d"[first_flag], next->layer_index);
                if (first_flag) { first_flag = false; }
            }
            printf("] # (%d, %d) -> %d\n", num, index, num + 1);
        }
    }



}

void Analyse::start_analyse(uint64_t code) {

    auto core = new_core();

    cases.empty();
    cache.empty();

    cases.reserve(65536);

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
//        .highlight = false,
//        .src = std::move(std::list<analyse_t*>{}),
    }).first->second);

    while (!cache.empty()) {

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

//    std::cout << "size: " << cases.size() << std::endl;

}

void Analyse::new_case(uint64_t code, uint64_t mask) {


//    auto temp = cases.emplace(code, analyse_t {
//        .code = code,
//        .mask = mask,
//        .step = cache.front()->step + 1,
//        .src = std::move(std::list<analyse_t*>{cache.front()})
//    });
//
//    if (temp.second) {
//        cache.emplace(&temp.first->second);
//    } else {
//        temp.first->second.mask |= mask;
//
//        if (temp.first->second.step != cache.front()->step) {
//            temp.first->second.src.push_back(cache.front());
//        }
//
//    }

    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info

        if (current->second.step != cache.front()->step) {
            current->second.src.push_back(cache.front());
        }

        return;
    }

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = mask,
        .step = cache.front()->step + 1,
//        .highlight = false,
        .src = std::move(std::list<analyse_t*>{cache.front()}),
    }).first->second);

}
