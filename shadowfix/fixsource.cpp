#include "plugin.h"

using namespace plugin;

class ShadowFix : public Plugin {
public:
	ShadowFix() : Plugin("ShadowFix") {}

	void OnPluginInit() override {
		Events::initGameEvent.Add(std::bind(&ShadowFix::OnGameInit, this));
	}

private:
	void OnGameInit() {
		auto shadowRenderFunction = Addresses::Get<void*>("0x400000");

		auto trampolineFunction = Trampoline::Create(shadowRenderFunction, 6);

		auto fixFunction = [](void* object) {
			((void(*)(void*))shadowRenderFunction)(object);

			auto shadowPosition = *(Vector3*)Addresses::Get<uint32_t>("0x8D0A88");
			shadowPosition.y -= 0.5f;
			*(Vector3*)Addresses::Get<uint32_t>("0x8D0A88") = shadowPosition;
		};
		memcpy(trampolineFunction, &fixFunction, 6);
		Memory::FlushInstructionCache();
	}
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL SupportsCLEO() {
	return CLEO_VERSION;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL SupportsASI() {
	return ASI_VERSION;
}

PLUGIN_EXPORT int PLUGIN_CALL Initialize() {
	ShadowFix().Register();
	return PLUGIN_OK;
}
