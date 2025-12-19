#include "plugin.hpp"
#include "lights.hpp"

struct ToggleModule : Module {
    enum ParamId {
		HOLD_PARAM,
        VALUE_PARAM,
		NUM_PARAMS
	};

    enum InputIds {
		TRIG_INPUT,
        VALUE_INPUT,
		NUM_INPUTS
	};

    enum OutputIds {
        OUT_OUTPUT,
        NUM_OUTPUTS
    };
    
	enum LightId {
		HOLD_LIGHT,
        LOGO_LIGHT,
		NUM_LIGHTS
	};

    ToggleModule() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configInput(TRIG_INPUT, "Trigger");
        configInput(VALUE_INPUT, "Mod");
        configOutput(OUT_OUTPUT, "Out");
		configButton(HOLD_PARAM, "Hold");
        configParam(VALUE_PARAM, 0.0f, 10.0f, 10.0f, "Value");

        lights[LOGO_LIGHT].setBrightness(1.0f);

        onReset();    
    }

    dsp::SchmittTrigger inputTrigger;
    dsp::BooleanTrigger holdBoolean;
    dsp::SchmittTrigger holdSchmitt;
    bool hold = false;

    void process(const ProcessArgs &args) override;

    void onReset() override {
		hold = false;
	}

    json_t* dataToJson() override {
		json_t *data, *hold_value;
		data = json_object();
		hold_value = json_boolean(hold);

		json_object_set(data, "hold", hold_value);

		json_decref(hold_value);
		return data;
	}

    void dataFromJson(json_t* root) override {
		json_t *hold_value;
		hold_value = json_object_get(root, "hold");

		if(json_is_boolean(hold_value)) {
			hold = json_boolean_value(hold_value);
		}
	}
};

void ToggleModule::process(const ProcessArgs &args) {
    if (holdBoolean.process(params[HOLD_PARAM].getValue()))
			hold ^= true;

    float scaledVoltage = rescale(inputs[TRIG_INPUT].getVoltage(), 0.1f, 2.0f, 0.0f, 1.0f);

    if (inputTrigger.process(scaledVoltage))
        hold = !hold;

    if (hold) {
        float val = params[VALUE_PARAM].getValue();
        float modVal = 0.0f;

        if (inputs[VALUE_INPUT].isConnected()) {
            modVal = inputs[VALUE_INPUT].getVoltage();
        }

        float outVal = clamp(val + modVal, 0.0f, 10.0f);
        outputs[OUT_OUTPUT].setVoltage(outVal);
    } else
        outputs[OUT_OUTPUT].setVoltage(0.0f);

    lights[HOLD_LIGHT].setBrightnessSmooth(hold, args.sampleTime);
}

struct ToggleModuleWidget : ModuleWidget {
    ToggleModuleWidget(ToggleModule *module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/ToggleModule.svg"),asset::plugin(pluginInstance, "res/ToggleModule-dark.svg")));

        addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addChild(createLightCentered<SmallSimpleLight<LempBlueLight>>(mm2px(Vec(9.15, 117.05)), module, ToggleModule::LOGO_LIGHT));

        addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<LempBlueLight>>>(Vec(22.5, 87), module, ToggleModule::HOLD_PARAM, ToggleModule::HOLD_LIGHT));        
        addParam(createParamCentered<Trimpot>(Vec(22.5, 202), module, ToggleModule::VALUE_PARAM));

        addInput(createInputCentered<PJ301MPort>(Vec(22.5, 125), module, ToggleModule::TRIG_INPUT));
        addInput(createInputCentered<PJ301MPort>(Vec(22.5, 240.5), module, ToggleModule::VALUE_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(Vec(22.5, 313), module, ToggleModule::OUT_OUTPUT));
    }
};

Model *modelToggleModule = createModel<ToggleModule, ToggleModuleWidget>("LempToggle");