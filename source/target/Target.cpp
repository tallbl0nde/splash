#include "target/Target.hpp"

// Constant values/indexes
#define TARGET_DARK_LUMA 0.26f
#define MAX_DARK_LUMA 0.45f
#define MIN_LIGHT_LUMA 0.55f
#define TARGET_LIGHT_LUMA 0.74f
#define MIN_NORMAL_LUMA 0.3f
#define TARGET_NORMAL_LUMA 0.5f
#define MAX_NORMAL_LUMA 0.7f
#define TARGET_MUTED_SATURATION 0.3f
#define MAX_MUTED_SATURATION 0.4f
#define TARGET_VIBRANT_SATURATION 1.0f
#define MIN_VIBRANT_SATURATION 0.35f
#define WEIGHT_SATURATION 0.24f
#define WEIGHT_LUMA 0.52f
#define WEIGHT_POPULATION 0.24f
#define INDEX_MIN 0
#define INDEX_TARGET 1
#define INDEX_MAX 2
#define INDEX_WEIGHT_SAT 0
#define INDEX_WEIGHT_LUMA 1
#define INDEX_WEIGHT_POP 2

namespace Splash::Target {
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

    float Target::getMinimumSaturation() const {
        return this->saturationTargets[INDEX_MIN];
    }

    float Target::getTargetSaturation() const {
        return this->saturationTargets[INDEX_TARGET];
    }

    float Target::getMaximumSaturation() const {
        return this->saturationTargets[INDEX_MAX];
    }

    float Target::getSaturationWeight() const {
        return this->weights[INDEX_WEIGHT_SAT];
    }

    float Target::getMinimumLightness() const {
        return this->lightnessTargets[INDEX_MIN];
    }

    float Target::getTargetLightness() const {
        return this->lightnessTargets[INDEX_TARGET];
    }

    float Target::getMaximumLightness() const {
        return this->lightnessTargets[INDEX_MAX];
    }

    float Target::getLightnessWeight() const {
        return this->weights[INDEX_WEIGHT_LUMA];
    }

    float Target::getPopulationWeight() const {
        return this->weights[INDEX_WEIGHT_POP];
    }

    bool Target::isExclusive() const {
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

    bool Target::operator==(const Target t) const {
        for (size_t i = 0; i < 3; i++) {
            if (this->lightnessTargets[i] != t.lightnessTargets[i]) {
                return false;
            }
            if (this->saturationTargets[i] != t.saturationTargets[i]) {
                return false;
            }
            if (this->weights[i] != t.weights[i]) {
                return false;
            }
        }

        return (this->isExclusive_ == t.isExclusive_);
    }
};