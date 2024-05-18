#pragma once

#include <bit>

namespace klotski::cases {

// TODO: using std::array
const uint16_t GROUP_NUM[203] = {
	1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
	1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,   13,    1,    1,    1,    1,
	1,    1,    1,    1,    1,    1,    1,    1,    5,    1,    1,    1,    1,    1,    1,    1,
	1,    1,    3,   79,    1,    1,    1,    1,    1,    1,    1,    1,    1,   21,  141,    1,
	1,    1,    1,    1,    1,    1,    1,    1,    1,   17,    1,    1,    1,    1,    1,    1,
	1,   17,  273,    1,    1,    1,    1,    1,    1,    9,  145,  841,    1,    1,    1,    1,
	1,    1,   15,   97,  517,    1,    1,    1,    1,    1,    1,    1,    5,   91,    1,    1,
	1,    1,    1,   59,  587,    1,    1,    1,    1,   21,  353, 1741,    1,    1,    1,    7,
   81,  479, 2327,    1,    1,    1,    5,   33,  165, 1053,    1,    1,    1,    1,    1,    9,
  157,    1,    1,    1,   57,  505,    1,    1,   13,  325, 1729,    3,    9,   99,  631, 2609,
	1,   17,   91,  487, 2653,    1,    1,   11,   75,  898,    2,    2,    2,    2,   80,    1,
	2,  138,   14,  126,  590,   22,  183, 1074,   33,  209, 1178,   13,  171, 1185,    6,   16,
  214,    6,   18,   54,    2,   44,   40,  124,   84,   70,   18,
};

const uint32_t GROUP_MAX_SIZE[203] = {
		12,    192,   1440,   6720,  21840,  52416,  96096, 137280, 154440, 137280,  96096,  52416,
	 21840,   6720,   1440,    124,   1736,  11284,  45136, 124124, 248248, 372372, 425568, 372372,
	248248, 124124,  45136,  10836,    132,   1848,  12012,  48048, 132132, 264264, 396396, 453024,
	396396, 264264, 132132,  48048,  11916,    512,   6144,  33792, 112640, 253440, 405504, 473088,
	405504, 253440, 112576,  30104,   1044,  12528,  68904, 229680, 516780, 826848, 964656, 826848,
	516780, 228848,  65848,    582,   6984,  38412, 128040, 288090, 460944, 537768, 460944, 288090,
	128040,  37740,   1092,  10920,  49140, 131040, 229320, 275184, 229320, 130032,  38672,   3180,
	 31800, 143100, 381600, 667800, 801360, 667544, 377408, 127676,   3382,  33820, 152190, 405840,
	710220, 852264, 709388, 403056, 143462,   1320,  13200,  59400, 158400, 277200, 332640, 277200,
	158272,  57524,   1320,  10560,  36960,  73920,  92400,  71944,  23704,   4680,  37440, 131040,
	262080, 327120, 253968, 105064,   7248,  57984, 202944, 405440, 504616, 396284, 175580,   5344,
	 42752, 149632, 299136, 373184, 296520, 136040,   1632,  13056,  45696,  91392, 114240,  91264,
	 43704,    948,   5688,  14220,  17696,   7888,   3444,  20664,  51044,  61204,  28832,   7048,
	 42288, 104540, 132556,  81462,   7284,  42728, 104960, 135192,  81340,   4392,  26352,  65688,
	 86968,  25955,    522,   3132,   7830,  10440,   7462,    396,    792,    508,    240,    768,
	   742,   1804,   2348,   1398,   1504,   5372,   1192,   2984,   4376,    453,    350,   1380,
	  1532,     45,     66,     38,     44,      5,     82,     21,     23,     24,     21,
};

const uint32_t GROUP_UNION_SIZE[203] = {
		12,    192,   1440,   6720,  21840,  52416,  96096, 137280, 154440, 137280,  96096,  52416,
	 21840,   6720,   1440,    124,   1736,  11284,  45136, 124124, 248248, 372372, 425568, 372372,
	248248, 124124,  45136,  11284,    132,   1848,  12012,  48048, 132132, 264264, 396396, 453024,
	396396, 264264, 132132,  48048,  12012,    512,   6144,  33792, 112640, 253440, 405504, 473088,
	405504, 253440, 112640,  33792,   1044,  12528,  68904, 229680, 516780, 826848, 964656, 826848,
	516780, 229680,  68904,    582,   6984,  38412, 128040, 288090, 460944, 537768, 460944, 288090,
	128040,  38412,   1092,  10920,  49140, 131040, 229320, 275184, 229320, 131040,  49140,   3180,
	 31800, 143100, 381600, 667800, 801360, 667800, 381600, 143100,   3382,  33820, 152190, 405840,
	710220, 852264, 710220, 405840, 152190,   1320,  13200,  59400, 158400, 277200, 332640, 277200,
	158400,  59400,   1320,  10560,  36960,  73920,  92400,  73920,  36960,   4680,  37440, 131040,
	262080, 327600, 262080, 131040,   7248,  57984, 202944, 405888, 507360, 405888, 202944,   5344,
	 42752, 149632, 299264, 374080, 299264, 149632,   1632,  13056,  45696,  91392, 114240,  91392,
	 45696,    948,   5688,  14220,  18960,  14220,   3444,  20664,  51660,  68880,  51660,   7120,
	 42720, 106800, 142400, 106800,   7284,  43704, 109260, 145680, 109260,   4392,  26352,  65880,
	 87840,  65880,   1044,   6264,  15660,  20880,  15660,    396,   1584,   2376,   1260,   5040,
	  7560,   3084,  12336,  18504,   4288,  17152,  25728,   3196,  12784,  19176,   1836,   7344,
	 11016,    270,   1080,   1620,     88,    180,    626,    772,   1036,    464,    342,
};

// ------------------------------------------------------------------------------------- //

inline uint32_t GroupUnion::unwrap() const {
	return type_id_;
}

inline GroupUnion GroupUnion::unsafe_create(const uint32_t type_id) {
	return std::bit_cast<GroupUnion>(type_id);
}

inline std::optional<GroupUnion> GroupUnion::create(const uint32_t type_id) {
	if (type_id < TYPE_ID_LIMIT) {
		return unsafe_create(type_id);
	}
	return std::nullopt;
}

// ------------------------------------------------------------------------------------- //

inline uint32_t GroupUnion::size() const {
	return GROUP_UNION_SIZE[type_id_];
}

inline uint32_t GroupUnion::group_num() const {
	return GROUP_NUM[type_id_];
}

inline uint32_t GroupUnion::max_group_size() const {
	return GROUP_MAX_SIZE[type_id_];
}

inline std::vector<Group> GroupUnion::groups() const {
	return {};
}

inline std::optional<Group> GroupUnion::group(const uint32_t group_id) const {
	if (group_id < GROUP_NUM[group_id]) {
		return Group::unsafe_create(type_id_, group_id);
	}
	return std::nullopt;
}

// ------------------------------------------------------------------------------------- //

inline GroupUnion GroupUnion::from_raw_code(const codec::RawCode raw_code) {
	return unsafe_create(type_id(raw_code));
}

inline GroupUnion GroupUnion::from_short_code(const codec::ShortCode short_code) {
	return from_common_code(short_code.to_common_code());
}

inline GroupUnion GroupUnion::from_common_code(const codec::CommonCode common_code) {
	return unsafe_create(type_id(common_code));
}

// ------------------------------------------------------------------------------------- //

} // namespace klotski::cases

namespace klotski::cases::internal {

class GroupImpl {
public:
    explicit GroupImpl(const int flat_id) : flat_id_(flat_id) {}

    const std::vector<codec::CommonCode>& cases();

    int flat_id_;
	bool available_;
	std::mutex building_;

    // static constexpr std::array<GroupImpl, ALL_GROUP_NUM> ins() {
    //     return std::array<GroupImpl, ALL_GROUP_NUM> {};
    // }
};

} // namespace klotski::cases::internal

// inline const std::vector<codec::CommonCode>& Group::cases() {
//     static auto kk = internal::GroupImpl::ins();
//     return kk[0].cases();
// }