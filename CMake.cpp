#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include <opencv2/opencv.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/dnn.h>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

// Dlib ResNet face recognition network
template <template<int, template<typename> class, int, typename> class block,
    int N, template<typename> class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N, BN, 1, dlib::tag1<SUBNET>>>;

template <template<int, template<typename> class, int, typename> class block,
    int N, template<typename> class BN, typename SUBNET>
using residual_down = dlib::add_prev2<
    dlib::avg_pool<2, 2, 2, 2,
    dlib::skip1<dlib::tag2<block<N, BN, 2, dlib::tag1<SUBNET>>>>>>;

template <int N, template<typename> class BN, int stride, typename SUBNET>
using block = BN<dlib::con<N, 3, 3, 1, 1,
    dlib::relu<BN<dlib::con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET>
using ares = dlib::relu<residual<block, N, dlib::affine, SUBNET>>;

template <int N, typename SUBNET>
using ares_down = dlib::relu<residual_down<block, N, dlib::affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = dlib::loss_metric<
    dlib::fc_no_bias<128,
    dlib::avg_pool_everything<
    alevel0<
    alevel1<
    alevel2<
    alevel3<
    alevel4<
    dlib::max_pool<3, 3, 2, 2,
    dlib::relu<dlib::affine<dlib::con<32, 7, 7, 2, 2,
    dlib::input_rgb_image_sized<150>
    >>>>>>>>>>>>;

struct Person {
    string name;
    dlib::matrix<float, 0, 1> descriptor;
};

double getDistance(
    const dlib::matrix<float, 0, 1>& a,
    const dlib::matrix<float, 0, 1>& b
) {
    return length(a - b);
}

int main() {
    try {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

        dlib::shape_predictor sp;
        dlib::deserialize("shape_predictor_5_face_landmarks.dat") >> sp;

        anet_type net;
        dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;

        vector<Person> knownPeople;

        for (const auto& entry : fs::directory_iterator("known")) {
            Mat img = imread(entry.path().string());

            if (img.empty()) {
                cout << "이미지 읽기 실패: " << entry.path() << endl;
                continue;
            }

            Mat rgb;
            cvtColor(img, rgb, COLOR_BGR2RGB);

            dlib::cv_image<dlib::rgb_pixel> dlibImg(rgb);

            vector<dlib::rectangle> faces = detector(dlibImg);

            if (faces.empty()) {
                cout << "등록 이미지에서 얼굴을 찾지 못함: " << entry.path() << endl;
                continue;
            }

            auto shape = sp(dlibImg, faces[0]);

            dlib::matrix<dlib::rgb_pixel> faceChip;
            dlib::extract_image_chip(
                dlibImg,
                dlib::get_face_chip_details(shape, 150, 0.25),
                faceChip
            );

            dlib::matrix<float, 0, 1> descriptor = net(faceChip);

            string name = entry.path().stem().string();
            knownPeople.push_back({ name, descriptor });

            cout << "등록 완료: " << name << endl;
        }

        VideoCapture cap("professor.mp4");
        if (!cap.isOpened()) {
            cout << "영상 파일을 열 수 없습니다." << endl;
            return -1;
        }

        while (true) {
            Mat frame;
            cap >> frame;

            for (int i = 0; i < 5; i++) {
                cap.grab();
            }

            if (frame.empty()) {
                cap.set(CAP_PROP_POS_FRAMES, 0);
                continue;
            }

            Mat rgbFrame;
            cvtColor(frame, rgbFrame, COLOR_BGR2RGB);

            dlib::cv_image<dlib::rgb_pixel> dlibFrame(rgbFrame);

            vector<dlib::rectangle> faces = detector(dlibFrame);

            for (const auto& face : faces) {
                auto shape = sp(dlibFrame, face);

                dlib::matrix<dlib::rgb_pixel> faceChip;
                dlib::extract_image_chip(
                    dlibFrame,
                    dlib::get_face_chip_details(shape, 150, 0.25),
                    faceChip
                );

                dlib::matrix<float, 0, 1> descriptor = net(faceChip);

                string resultName = "Unknown";
                double bestDistance = 999.0;

                for (const auto& person : knownPeople) {
                    double dist = getDistance(descriptor, person.descriptor);

                    if (dist < bestDistance) {
                        bestDistance = dist;
                        resultName = person.name;
                    }
                }

                if (bestDistance > 0.6) {
                    resultName = "Unknown";
                }

                rectangle(
                    frame,
                    Point((int)face.left(), (int)face.top()),
                    Point((int)face.right(), (int)face.bottom()),
                    Scalar(0, 255, 0),
                    2
                );

                putText(
                    frame,
                    resultName + " dist=" + to_string(bestDistance).substr(0, 4),
                    Point((int)face.left(), max(20, (int)face.top() - 10)),
                    FONT_HERSHEY_SIMPLEX,
                    0.7,
                    Scalar(0, 255, 0),
                    2
                );

                putText(
                    frame,
                    "128D face vector extracted",
                    Point(20, 40),
                    FONT_HERSHEY_SIMPLEX,
                    0.8,
                    Scalar(255, 0, 0),
                    2
                );
            }

            imshow("OpenCV + Dlib Face Recognition Demo", frame);

            if (waitKey(30) == 27) {
                break;
            }
        }

        cap.release();
        destroyAllWindows();
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return -1;
    }

    return 0;
}
