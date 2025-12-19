#pragma once
#include <rack.hpp>

static const NVGcolor SCHEME_LEMP_BLUE = nvgRGB(0x00, 0xCC, 0xFF);

template <typename TBase = GrayModuleLightWidget>
struct TLempBlueLight : TBase {
	TLempBlueLight() {
		this->addBaseColor(SCHEME_LEMP_BLUE);
	}
};

using LempBlueLight = TLempBlueLight<>;