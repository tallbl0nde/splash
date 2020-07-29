#include "target/Target.hpp"

namespace Palette::Target {
    // Constant values/indexes
    static float TARGET_DARK_LUMA = 0.26f;
    static float MAX_DARK_LUMA = 0.45f;
    static float MIN_LIGHT_LUMA = 0.55f;
    static float TARGET_LIGHT_LUMA = 0.74f;
    static float MIN_NORMAL_LUMA = 0.3f;
    static float TARGET_NORMAL_LUMA = 0.5f;
    static float MAX_NORMAL_LUMA = 0.7f;
    static float TARGET_MUTED_SATURATION = 0.3f;
    static float MAX_MUTED_SATURATION = 0.4f;
    static float TARGET_VIBRANT_SATURATION = 1.0f;
    static float MIN_VIBRANT_SATURATION = 0.35f;
    static float WEIGHT_SATURATION = 0.24f;
    static float WEIGHT_LUMA = 0.52f;
    static float WEIGHT_POPULATION = 0.24f;
    static int INDEX_MIN = 0;
    static int INDEX_TARGET = 1;
    static int INDEX_MAX = 2;
    static int INDEX_WEIGHT_SAT = 0;
    static int INDEX_WEIGHT_LUMA = 1;
    static int INDEX_WEIGHT_POP = 2;

    void Target::setDefaultLightLightnessValues() {
        this->lightnessTargets[INDEX_MIN] = MIN_LIGHT_LUMA;
        this->lightnessTargets[INDEX_TARGET] = TARGET_LIGHT_LUMA;
    }

    void Target::setDefaultNormalLightnessValues() {
        this->lightnessTargets[INDEX_MIN] = MIN_NORMAL_LUMA;
        this->lightnessTargets[INDEX_TARGET] = TARGET_NORMAL_LUMA;
        this->lightnessTargets[INDEX_MAX] = MAX_NORMAL_LUMA;
    }

    void Target::setDefaultDarkLightnessValues() {
        this->lightnessTargets[INDEX_TARGET] = TARGET_DARK_LUMA;
        this->lightnessTargets[INDEX_MAX] = MAX_DARK_LUMA;
    }

    void Target::setDefaultVibrantSaturationValues() {
        this->saturationTargets[INDEX_MIN] = MIN_VIBRANT_SATURATION;
        this->saturationTargets[INDEX_TARGET] = TARGET_VIBRANT_SATURATION;
    }

    void Target::setDefaultMutedSaturationValues() {
        this->saturationTargets[INDEX_TARGET] = TARGET_MUTED_SATURATION;
        this->saturationTargets[INDEX_MAX] = MAX_MUTED_SATURATION;
    }

    Target::Target() {
        this->setDefaultWeights();
        this->setTargetDefaultValues(this->lightnessTargets);
        this->setTargetDefaultValues(this->saturationTargets);
    }

    Target::Target(const Target & t) {
        this->lightnessTargets = t.lightnessTargets;
        this->saturationTargets = t.saturationTargets;
        this->weights = t.weights;
    }

    Target::Target(const Target * t) {
        this->lightnessTargets = t->lightnessTargets;
        this->saturationTargets = t->saturationTargets;
        this->weights = t->weights;
    }

    void Target::setDefaultWeights() {
        this->weights[INDEX_WEIGHT_SAT] = WEIGHT_SATURATION;
        this->weights[INDEX_WEIGHT_LUMA] = WEIGHT_LUMA;
        this->weights[INDEX_WEIGHT_POP] = WEIGHT_POPULATION;
    }

    void Target::setTargetDefaultValues(std::array<float, 3> & arr) {
        arr[INDEX_MIN] = 0.0f;
        arr[INDEX_TARGET] = 0.5f;
        arr[INDEX_MAX] = 1.0f;
    }

    float Target::getMinimumSaturation() {
        return this->saturationTargets[INDEX_MIN];
    }

    float Target::getTargetSaturation() {
        return this->saturationTargets[INDEX_TARGET];
    }

    float Target::getMaximumSaturation() {
        return this->saturationTargets[INDEX_MAX];
    }

    float Target::getSaturationWeight() {
        return this->weights[INDEX_WEIGHT_SAT];
    }

    float Target::getMinimumLightness() {
        return this->lightnessTargets[INDEX_MIN];
    }

    float Target::getTargetLightness() {
        return this->lightnessTargets[INDEX_TARGET];
    }

    float Target::getMaximumLightness() {
        return this->lightnessTargets[INDEX_MAX];
    }

    float Target::getLightnessWeight() {
        return this->weights[INDEX_WEIGHT_LUMA];
    }

    float Target::getPopulationWeight() {
        return this->weights[INDEX_WEIGHT_POP];
    }

    bool Target::isExclusive() {
        return this->isExclusive_;
    }

    void Target::normalizeWeights() {
        float sum = 0;
        for (size_t i = 0; i < this->weights.size(); i++) {
            float weight = this->weights[i];
            if (weight > 0) {
                sum += weight;
            }
        }
        if (sum != 0) {
            for (size_t i = 0; i < this->weights.size(); i++) {
                if (this->weights[i] > 0) {
                    this->weights[i] /= sum;
                }
            }
        }
    }

    Target::Builder::Builder() {
        this->target = new Target();
    }

    Target::Builder::Builder(Target & t) {
        this->target = new Target(t);
    }

    Target::Builder & Target::Builder::setMinimumSaturation(float v) {
        this->target->saturationTargets[INDEX_MIN] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setTargetSaturation(float v) {
        this->target->saturationTargets[INDEX_TARGET] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setMaximumSaturation(float v) {
        this->target->saturationTargets[INDEX_MAX] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setSaturationWeight(float v) {
        this->target->weights[INDEX_WEIGHT_SAT] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setMinimumLightness(float v) {
        this->target->lightnessTargets[INDEX_MIN] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setTargetLightness(float v) {
        this->target->lightnessTargets[INDEX_TARGET] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setMaximumLightness(float v) {
        this->target->lightnessTargets[INDEX_MAX] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setLightnessWeight(float v) {
        this->target->weights[INDEX_WEIGHT_LUMA] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setPopulationWeight(float v) {
        this->target->weights[INDEX_WEIGHT_POP] = v;
        return *this;
    }

    Target::Builder & Target::Builder::setExclusive(bool b) {
        this->target->isExclusive_ = b;
        return *this;
    }

    Target Target::Builder::build() {
        return Target(this->target);
    }

    Target::Builder::~Builder() {
        delete this->target;
    }
};