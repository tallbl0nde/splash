#ifndef SPLASH_TARGET_TARGET_HPP
#define SPLASH_TARGET_TARGET_HPP

#include <array>

namespace Splash::Target {
    // A Target allows custom selection of colours in a Palette's generation
    class Target {
        private:
            // Is any colour selected for this target exclusive to this target?
            bool isExclusive_;

            // Initialize weights array with default values
            void setDefaultWeights();
            // Initialize given array with default values
            void setTargetDefaultValues(std::array<float, 3> &);


        protected:
            // Arrays of target values
            std::array<float, 3> lightnessTargets;
            std::array<float, 3> saturationTargets;
            std::array<float, 3> weights;

            // Functions called by derived instances to set up their values
            void setDefaultLightLightnessValues();
            void setDefaultNormalLightnessValues();
            void setDefaultDarkLightnessValues();
            void setDefaultVibrantSaturationValues();
            void setDefaultMutedSaturationValues();

        public:
            // Constructor creates target but doesn't set values
            Target();

            // Copy constructors
            Target(const Target &);
            Target(const Target *);

            // Returns the minimum saturation value of the target
            float getMinimumSaturation() const;
            // Returns the target saturation value for the target
            float getTargetSaturation() const;
            // Returns the maximum saturation value of this target
            float getMaximumSaturation() const;
            // Return the weight of importance the target places on
            // the image's colour saturation
            float getSaturationWeight() const;

            // Returns the minimum lightness value of the target
            float getMinimumLightness() const;
            // Returns the target lightness value for the target
            float getTargetLightness() const;
            // Returns the maximum lightness value of this target
            float getMaximumLightness() const;
            // Return the weight of importance the target places on
            // the image's colour lightness
            float getLightnessWeight() const;

            // Return the weight of importance the target places on
            // the image's colour population
            float getPopulationWeight() const;

            // Return whether any colour selected for this target is
            // exclusive to the target
            bool isExclusive() const;

            // Normalize the target's weights
            void normalizeWeights();

            // Nested builder class
            class Builder {
                private:
                    Target * target;

                public:
                    // Create a target from scratch (default values)
                    Builder();

                    // Create a new builder based on an existing target
                    Builder(Target &);

                    // Set the minimum saturation value of the target
                    Builder & setMinimumSaturation(float);
                    // Set the target saturation value for the target
                    Builder & setTargetSaturation(float);
                    // Set the maximum saturation value of this target
                    Builder & setMaximumSaturation(float);
                    // Set the weight of importance the target places on
                    // the image's colour saturation
                    Builder & setSaturationWeight(float);

                    // Set the minimum lightness value of the target
                    Builder & setMinimumLightness(float);
                    // Set the target lightness value for the target
                    Builder & setTargetLightness(float);
                    // Set the maximum lightness value of this target
                    Builder & setMaximumLightness(float);
                    // Set the weight of importance the target places on
                    // the image's colour lightness
                    Builder & setLightnessWeight(float);

                    // Set the weight of importance the target places on
                    // the image's colour population
                    Builder & setPopulationWeight(float);

                    // Set whether any colour selected for this target is
                    // exclusive to the target
                    Builder & setExclusive(bool);

                    // Build and return the created target
                    Target build();

                    // Delete the Target object stored on the heap
                    ~Builder();
            };

        // Equality operator
        bool operator==(const Target) const;
    };

};

#endif