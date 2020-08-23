
#include <config.h>
#include "ui/AnimatedAtlas.h"

Animation::Animation(int * frames, int frameCount, int frameSpeed, bool loop) {
    this->frames = frames;
    this->frameCount = frameCount;
    this->frameSpeed = frameSpeed;
    this->loop = loop;

    this->isComplete = false;
    this->currentFrameIndex = 0;
    this->lastFrameChangeMillis = millis();
}

Animation::Animation(animationData data, bool loop) :
    Animation(data.frames,data.frameCount,data.frameSpeed,loop) 
{

}

//Update loop
void Animation::update() {

    //No animation necessary
    //if(this->isComplete || this->frameCount == 0)
    //    return;

    int diff = millis() - this->lastFrameChangeMillis;
    
    //Detect frame change
    if(diff > this->frameSpeed) {
        
        //bring back to 0
        if(this->currentFrameIndex < this->frameCount - 1)
            this->currentFrameIndex += 1;
        
        //Detect end of animation
        else {
            if(this->loop)
                this->currentFrameIndex = 0;
            else
                this->isComplete = true;
        }
        
        this->lastFrameChangeMillis = millis();
    }

}

//Returns the current animation's frame value based on the current frame index
int Animation::getCurrentFrame() {
    return this->frames[this->currentFrameIndex];
}

AnimatedAtlas::AnimatedAtlas(lv_obj_t* image, int width, int height) {

    this->image = image;
    this->width = width;
    this->height = height;

    lv_obj_set_size(this->image, width, height);
    this->setAtlasFrame(0);
}

void AnimatedAtlas::PlayAnimation(animationData animation) {
    this->currentAnimation = new Animation(animation);
}

void AnimatedAtlas::PlayOverlayAnimation(animationData animation) {
    this->currentOverlayAnimation = new Animation(animation,false);
}

void AnimatedAtlas::update() {
    
    if(this->currentOverlayAnimation != nullptr) {
        this->currentOverlayAnimation->update();
        
        //Detect completion
        if(this->currentOverlayAnimation->isComplete)
            this->currentOverlayAnimation = nullptr;
        //Update visual
        else
            this->setAtlasFrame(this->currentOverlayAnimation->getCurrentFrame());
    }

    else if(this->currentAnimation != nullptr) {
        //Update Visual
        this->currentAnimation->update();
        this->setAtlasFrame(this->currentAnimation->getCurrentFrame());
    }
}

void AnimatedAtlas::setAtlasFrame(int frame) {

    //Only update offset if needed, since LVGL seems to redraw stuff
    if(frame != this->currentFrame) {
        lv_img_set_offset_x(this->image,this->width * -frame);
        this->currentFrame = frame;
    }

}