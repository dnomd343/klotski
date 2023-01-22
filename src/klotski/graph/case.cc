#include "graph.h"
#include "common.h"

void GraphCase::render(SvgGraph &svg, const CaseSkin &skin) const {
    /// precompute size info
    auto raw_code = code;
    uint32_t block_width_2 = block_width * 2 + block_gap;
    uint32_t case_width = block_width * 4 + block_gap * 5;
    uint32_t case_height = block_width * 5 + block_gap * 6;

    /// case skeleton render
    auto skeleton = new SvgRect {start, case_width, case_height};
    skeleton->color = skin.CASE_BG_COLOR;
    skeleton->stroke = skin.CASE_BORDER_WIDTH;
    skeleton->line_color = skin.CASE_BORDER_COLOR;
    skeleton->radius = uint64_t(skin.CASE_RADIUS * (float)block_width);
    svg.insert(skeleton);

    /// lambda for insert new block
    auto new_block = [this, &skin, &svg](SvgRect *block) {
        block->color = skin.BLOCK_BG_COLOR;
        block->stroke = skin.BLOCK_BORDER_WIDTH;
        block->line_color = skin.BLOCK_BORDER_COLOR;
        block->radius = uint64_t(skin.BLOCK_RADIUS * (float)block_width);
        svg.insert(block);
    };

    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
        /// calculate block address
        uint32_t block_x = addr % 4;
        uint32_t block_y = (addr - block_x) / 4;
        Point block_start = {
            start.x + block_x * block_width + (block_x + 1) * block_gap,
            start.y + block_y * block_width + (block_y + 1) * block_gap,
        };
        /// render into svg graph
        switch (raw_code & 0b111) {
            case B_1x1:
                new_block(new SvgRect {block_start, block_width, block_width});
                break;
            case B_1x2:
                new_block(new SvgRect {block_start, block_width_2, block_width});
                break;
            case B_2x1:
                new_block(new SvgRect {block_start, block_width, block_width_2});
                break;
            case B_2x2:
                new_block(new SvgRect {block_start, block_width_2, block_width_2});
                break;
            default:
                continue;
        }
    }
}
