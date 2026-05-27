# Facial-Recognition-Project
- 권준욱(20250100)🌟
- 변수경(20250636)
- 송병헌(20250699)
- 황은영(20240961)

## 📌 Project Overview

본 프로젝트는 현대적 C++ 문법과 OpenCV, Dlib 딥러닝 라이브러리를 활용하여 다음 기능을 구현합니다.

- 영상 파일 입력 처리
- 얼굴 탐지 (Face Detection)
- 얼굴 Landmark 기반 정렬
- 128D Face Embedding 추출
- 유클리드 거리 기반 인물 분류

---

## 🛠 Development Environment

| Category | Description |
|---|---|
| Language | C++17 |
| IDE | Visual Studio 2026 |
| Build System | CMake |
| Package Manager | vcpkg |
| Library | OpenCV |
| Library | Dlib |

---

## 📂 Project Structure

```txt
CMakeProject1/
 ├─ CMakeLists.txt
 ├─ CMakeProject1.cpp
 ├─ professor.mp4
 ├─ shape_predictor_5_face_landmarks.dat
 ├─ dlib_face_recognition_resnet_model_v1.dat
 ├─ known/
 │   └─ professor.png
 └─ out/
```

---

## ⚙ Main Features

### 1️⃣ Video Input & Preprocessing (OpenCV)

- `VideoCapture`를 사용하여 영상 파일 입력 처리
- OpenCV BGR → RGB 변환 수행
- 실시간 프레임 처리

```cpp
auto cap = std::make_unique<VideoCapture>("professor.mp4");

cvtColor(frame, rgbFrame, COLOR_BGR2RGB);
```

---

### 2️⃣ Face Detection & Alignment (Dlib)

- Dlib HOG Face Detector 사용
- 5-point Landmark 모델 사용
- `extract_image_chip()` 기반 얼굴 정렬

```cpp
auto shape = (*sp)(dlibFrame, face);

dlib::extract_image_chip(
    dlibFrame,
    dlib::get_face_chip_details(shape, 150, 0.25),
    faceChip
);
```

---

### 3️⃣ 128D Face Embedding & Classification

- Dlib ResNet 기반 128차원 벡터 추출
- 유클리드 거리 계산 기반 인물 분류
- 동일인 여부 판별

```cpp
dlib::matrix<float, 0, 1> descriptor = (*net)(faceChip);

double dist = length(descriptor - person.descriptor);
```

---

## 🧠 Recognition Pipeline

```txt
Video Input
    ↓
Face Detection
    ↓
Landmark Detection
    ↓
Face Alignment
    ↓
128D Embedding Extraction
    ↓
Euclidean Distance Comparison
    ↓
Person Classification
```

---

## 🚀 Build Instructions

### 1. Install vcpkg

```bash
git clone https://github.com/microsoft/vcpkg
```

---

### 2. Install OpenCV & Dlib

```bash
vcpkg install opencv4:x64-windows
vcpkg install dlib:x64-windows
```

---

### 3. Configure CMake

```cmake
find_package(OpenCV REQUIRED)
find_package(dlib CONFIG REQUIRED)
```

---

### 4. Build

```txt
Build → Build All
```

---

## 📊 Result

- 얼굴 탐지 성공
- 얼굴 정렬 성공
- 128D 벡터 추출 성공
- 실시간 인물 분류 성공

Example Output:

```txt
professor dist=0.04
128D face vector extracted
```

---

## 💡 Modern C++ Features

본 프로젝트에서는 modern C++ 스타일을 적용하기 위해 `std::unique_ptr`, `std::shared_ptr`, `std::move`를 활용하였다.

- `std::unique_ptr`
  - `VideoCapture` 객체의 RAII 기반 자원 관리 수행

```cpp
auto cap = std::make_unique<VideoCapture>("professor.mp4");
```

- `std::shared_ptr`
  - Dlib detector, shape predictor, ResNet 모델 객체 공유 관리

```cpp
auto detector = std::make_shared<dlib::frontal_face_detector>(
    dlib::get_frontal_face_detector()
);
```

- `std::move`
  - 불필요한 객체 복사 최소화 및 메모리 오버헤드 감소

```cpp
knownPeople.emplace_back(std::move(name), std::move(descriptor));
```

---

## 💡 Optimization

본 프로젝트에서는 다음과 같은 최적화 기법을 고려하였다.

- Release Mode Build
- OpenCV DLL 분리
- 프레임 스킵 처리
- Dlib 기반 얼굴 칩 정렬
- Move Semantics 기반 메모리 최적화

---

## 📚 References

- https://github.com/opencv/opencv
- https://github.com/davisking/dlib
- https://github.com/davisking/dlib-models
- https://github.com/microsoft/vcpkg
- http://dlib.net/dnn_face_recognition_ex.cpp.html
