//
// Created by wyz on 2021/7/3.
//

#ifndef CGUTILS_CAMERA_HPP
#define CGUTILS_CAMERA_HPP

#include <array>

struct alignas(16) Camera{
    std::array<float,3> camera_pos;
    float n;
    std::array<float,3> camera_look_at;
    float f;
    std::array<float,3> camera_up;
    float zoom;
};
class AbstractCamera{
public:
    AbstractCamera()=default;
    virtual Camera GetCamera() const =0;

};
class TrackBallCamera: public AbstractCamera{

};
class FPSCamera: public AbstractCamera{

};



#endif //CGUTILS_CAMERA_HPP
