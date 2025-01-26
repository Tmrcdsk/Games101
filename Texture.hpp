//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        u_img = std::max(0.0f, std::min(u_img, static_cast<float>(width - 1)));
        v_img = std::max(0.0f, std::min(v_img, static_cast<float>(height - 1)));

        // 确定四个相邻像素的整数坐标
        int x0 = static_cast<int>(std::floor(u_img));
        int x1 = std::min(x0 + 1, width - 1);
        int y0 = static_cast<int>(std::floor(v_img));
        int y1 = std::min(y0 + 1, height - 1);

        // 获取四个相邻像素的颜色（注意参数顺序为(y, x)）
        cv::Vec3b a = image_data.at<cv::Vec3b>(y0, x0);
        cv::Vec3b b = image_data.at<cv::Vec3b>(y0, x1);
        cv::Vec3b c = image_data.at<cv::Vec3b>(y1, x0);
        cv::Vec3b d = image_data.at<cv::Vec3b>(y1, x1);

        // 转换为浮点数进行插值计算
        cv::Vec3f a_f(a[0], a[1], a[2]);
        cv::Vec3f b_f(b[0], b[1], b[2]);
        cv::Vec3f c_f(c[0], c[1], c[2]);
        cv::Vec3f d_f(d[0], d[1], d[2]);

        auto e_color = a_f + (u_img - x0) * (b_f - a_f);
        auto f_color = c_f + (u_img - x0) * (d_f - c_f);

        auto color = e_color + (v_img - y0) * (f_color - e_color);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
