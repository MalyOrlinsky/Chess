#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include <functional>

class Img {
public:
    Img();

    Img(const Img& other);
    Img& operator=(const Img& other);

    Img& read(const std::string& path,
              const std::pair<int, int>& size = {},
              bool keep_aspect = false,
              int interpolation = cv::INTER_AREA);

    void draw_on(Img& other_img, int x, int y);

    void put_text(const std::string& txt, int x, int y, double font_size,
                  const cv::Scalar& color = cv::Scalar(255, 255, 255, 255),
                  int thickness = 1);

    // Show once (blocking)
    void show();

    // Show in named window (non-blocking)
    void show_in(const std::string& window_name);

    // Create named window
    static void create_window(const std::string& window_name);

    // Set mouse callback on a named window
    // callback receives (event, x, y, userdata)
    static void set_mouse_callback(const std::string& window_name,
                                   cv::MouseCallback callback,
                                   void* userdata);

    // Wait for key, returns key code (-1 if timeout)
    static int wait_key(int delay_ms);

    static void destroy_windows();

    const cv::Mat& get_mat() const { return img; }
    bool is_loaded() const { return !img.empty(); }

    void draw_rectangle(int x, int y, int width, int height, const cv::Scalar& color, int thickness = 1);
    static bool is_window_open(const std::string& window_name);
private:
    cv::Mat img;
};
