#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
        0, 1, 0, -eye_pos[1],
        0, 0, 1, -eye_pos[2],
        0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    rotation_angle *= MY_PI / 180.;
    float s = sin(rotation_angle), c = cos(rotation_angle);
    model << c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    return model;
}

// fov: field of view
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // Create the projection matrix for the given parameters.
    // Then return it.
    Eigen::Matrix4f persp_ortho, ortho;
    persp_ortho << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;
    float t = tan(eye_fov / 2.) * zNear, r = aspect_ratio * t;
    ortho << 1.0 / r, 0, 0, 0,
        0, 1.0 / t, 0, 0,
        0, 0, 2 / (zNear - zFar), 0,
        0, 0, 0, 1;
    projection = ortho * persp_ortho;

    return projection;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle) {
    axis /= axis.norm(); // unit
    float x = axis[0], y = axis[1], z = axis[2];
    float x2 = x * x, y2 = y * y, z2 = z * z;
    float xy = x * y, yz = y * z, zx = z * x;
    float theta = angle / 180. * MY_PI;
    float c = cos(theta), c1 = 1 - c, s = sin(theta);
    Eigen::Matrix4f m;
    m << c + c1 * x2, c1 *xy - s * z, c1 *zx + s * y, 0,
        c1 *xy + s * z, c + c1 * y2, c1 *yz - s * x, 0,
        c1 *zx - s * y, c1 *yz + s * x, c + c1 * z2, 0,
        0, 0, 0, 1;
    return m;
}

int main(int argc, const char **argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        } else {
            return 0;
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector < Eigen::Vector3f > pos {{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector < Eigen::Vector3i > ind {{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (!(key == 27 || key == 'q')) {  /** Esc key to exit */
        // r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // r.set_model(get_model_matrix(angle));
        Eigen::Vector3f axis;
        axis << 0.5, 0.5, 1;
        r.set_model(get_rotation(axis, angle));

        r.set_view(get_view_matrix(eye_pos));

        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        // std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 5;
        } else if (key == 'd') {
            angle -= 5;
        }
        angle += 1;
    }

    return 0;
}
