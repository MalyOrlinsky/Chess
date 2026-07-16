#include "img.hpp"
#include <stdexcept>

Img::Img() {}

Img::Img(const Img& other) {
    img = other.img.clone();
}

Img& Img::operator=(const Img& other) {
    if (this != &other)
        img = other.img.clone();
    return *this;
}

Img& Img::read(const std::string& path,
               const std::pair<int, int>& size,
               bool keep_aspect,
               int interpolation) {
    img = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (img.empty())
        throw std::runtime_error("Cannot load image: " + path);

    if (img.channels() == 3)
        cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);

    if (size.first != 0 && size.second != 0) {
        if (keep_aspect) {
            double scale = std::min((double)size.first  / img.cols,
                                    (double)size.second / img.rows);
            cv::resize(img, img,
                cv::Size((int)(img.cols * scale), (int)(img.rows * scale)),
                0, 0, interpolation);
        } else {
            cv::resize(img, img, cv::Size(size.first, size.second), 0, 0, interpolation);
        }
    }
    return *this;
}

void Img::draw_on(Img& other_img, int x, int y) {
    if (img.empty() || other_img.img.empty())
        throw std::runtime_error("Both images must be loaded before drawing.");

    int h = img.rows, w = img.cols;
    int H = other_img.img.rows, W = other_img.img.cols;

    if (y + h > H || x + w > W)
        throw std::runtime_error("Image does not fit at the specified position.");

    cv::Mat roi = other_img.img(cv::Rect(x, y, w, h));

    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            cv::Vec4b  s = img.at<cv::Vec4b>(r, c);
            cv::Vec4b& d = roi.at<cv::Vec4b>(r, c);
            float a = s[3] / 255.0f;
            d[0] = (uchar)(s[0] * a + d[0] * (1.0f - a));
            d[1] = (uchar)(s[1] * a + d[1] * (1.0f - a));
            d[2] = (uchar)(s[2] * a + d[2] * (1.0f - a));
            d[3] = 255;
        }
    }
}

void Img::put_text(const std::string& txt, int x, int y, double font_size,
                   const cv::Scalar& color, int thickness) {
    if (img.empty())
        throw std::runtime_error("Image not loaded.");
    cv::putText(img, txt, cv::Point(x, y),
                cv::FONT_HERSHEY_SIMPLEX, font_size,
                color, thickness, cv::LINE_AA);
}

void Img::show() {
    if (img.empty())
        throw std::runtime_error("Image not loaded.");
    cv::imshow("Image", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void Img::show_in(const std::string& window_name) {
    if (img.empty())
        throw std::runtime_error("Image not loaded.");
    cv::imshow(window_name, img);
}

void Img::create_window(const std::string& window_name) {
    cv::namedWindow(window_name);
}

void Img::set_mouse_callback(const std::string& window_name,
                              cv::MouseCallback callback,
                              void* userdata) {
    cv::setMouseCallback(window_name, callback, userdata);
}

int Img::wait_key(int delay_ms) {
    return cv::waitKey(delay_ms);
}

void Img::destroy_windows() {
    cv::destroyAllWindows();
}
