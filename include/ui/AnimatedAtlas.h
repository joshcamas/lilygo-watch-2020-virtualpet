#pragma once

#include <lvgl.h>

typedef struct {
    int * frames;
    int frameCount;
    int frameSpeed;
} animationData;

class Animation {
    private:

        int * frames;
        int frameCount;
        int frameSpeed;
        
        int currentFrameIndex;
        int lastFrameChangeMillis;
        bool loop;

    public:

        int getCurrentFrame();    
        bool isComplete;

        Animation(int * frames, int frameCount, int frameSpeed = 1000, bool loop = true);
        
        Animation(animationData data, bool loop = true);
        
        void update();
};

class AnimatedAtlas {
    private:
        lv_obj_t* image = nullptr;

        Animation* currentAnimation = nullptr;
        Animation* currentOverlayAnimation = nullptr;

        int currentFrame;
        int width;
        int height;
        
        void setAtlasFrame(int frame);
    public:

        AnimatedAtlas(lv_obj_t* image, int width, int height);

        void PlayAnimation(animationData animation);
        void PlayOverlayAnimation(animationData animation);

        void update();
};