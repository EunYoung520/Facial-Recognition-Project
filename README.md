# Facial-Recognition-Project
- 권준욱(20250100)🌟
- 변수경(20250636)
- 송병헌(20250699)
- 황은영(20240961)

## 🎯 Project Goals

- 영상 속 얼굴 탐지
- 얼굴 Landmark 추출
- 얼굴 정렬(Face Alignment)
- 128차원 얼굴 특징 벡터 생성
- 유클리드 거리 기반 얼굴 비교
- 여러 사람 분류 및 인식

---

## 🛠 Development Environment

| Category | Description |
|---|---|
| Language | C++17 |
| IDE | Visual Studio 2026 |
| Build System | CMake |
| Package Manager | vcpkg |
| Libraries | OpenCV, dlib |

---

## 📂 Project Structure

```txt
CMakeProject1/
 ├─ CMakeLists.txt
 ├─ CMakeProject1.cpp
 ├─ teachers_day.mp4
 ├─ shape_predictor_5_face_landmarks.dat
 ├─ dlib_face_recognition_resnet_model_v1.dat
 ├─ known/
 │   ├─ BAEG.png
 │   ├─ CHA.png
 │   ├─ LEE.png
 │   ├─ SIM.png
 │   └─ YANG.png
```

---

## ⚙ System Pipeline

```txt
Video Input
    ↓
Face Detection
    ↓
Landmark Extraction
    ↓
Face Alignment
    ↓
128D Face Embedding Extraction
    ↓
Euclidean Distance Comparison
    ↓
Person Classification
```

---

## 📷 OpenCV 역할

OpenCV는 영상 입력 및 화면 출력을 담당한다.

### 주요 기능

- `VideoCapture` 기반 영상 입력
- 실시간 프레임 처리
- BGR → RGB 색상 변환
- Bounding Box 출력
- 이름 및 거리값 출력

```cpp
auto cap = std::make_unique<VideoCapture>("teachers_day.mp4");

cvtColor(frame, rgbFrame, COLOR_BGR2RGB);
```

---

## 🧠 dlib 역할

dlib은 얼굴 탐지 및 딥러닝 기반 얼굴 인식을 담당한다.

### 사용 기능

- `frontal_face_detector`
  - 얼굴 영역 탐지

- `shape_predictor`
  - 얼굴 Landmark 추출

- `extract_image_chip()`
  - 얼굴 정렬(Face Alignment)

- ResNet 기반 Face Recognition Model
  - 128차원 얼굴 특징 벡터 생성

---

## 📍 Landmark 기반 얼굴 정렬

본 프로젝트에서는 얼굴 인식 정확도 향상을 위해 Landmark 기반 얼굴 정렬(Face Alignment)을 수행하였다.

눈, 코, 입 위치를 기준으로 얼굴 방향을 정렬하여 동일한 형태의 얼굴 이미지를 생성하였다.

```cpp
auto shape = (*sp)(dlibFrame, face);

dlib::extract_image_chip(
    dlibFrame,
    dlib::get_face_chip_details(shape, 150, 0.25),
    faceChip
);
```

---

## 🔍 128D Face Embedding

정렬된 얼굴 이미지는 dlib의 ResNet 기반 신경망을 통해 128차원 특징 벡터로 변환된다.

생성된 벡터 간 유클리드 거리를 계산하여 동일 인물 여부를 판별하였다.

```cpp
dlib::matrix<float, 0, 1> descriptor = (*net)(faceChip);

double dist = length(descriptor - person.descriptor);
```

---

## 👥 Multiple Person Recognition

본 프로젝트는 단일 인물 인식이 아닌 여러 사람을 구분할 수 있도록 구현하였다.

### 등록 인물

- BAEG
- CHA
- LEE
- SIM
- YANG

### 동작 방식

known 폴더에 등록된 얼굴 이미지와 영상 속 얼굴을 비교하여 가장 가까운 얼굴을 선택한다.

```txt
BAEG dist=0.31
CHA dist=0.27
```

---

## 💡 Modern C++ Features

본 프로젝트에서는 modern C++ 스타일을 적용하기 위해 `std::unique_ptr`, `std::shared_ptr`, `std::move`를 활용하였다.

---

### ✅ std::unique_ptr

`VideoCapture` 객체를 RAII 기반으로 안전하게 관리하였다.

```cpp
auto cap = std::make_unique<VideoCapture>("teachers_day.mp4");
```

---

### ✅ std::shared_ptr

dlib detector 및 모델 객체를 공유 관리하였다.

```cpp
auto detector = std::make_shared<dlib::frontal_face_detector>(
    dlib::get_frontal_face_detector()
);
```

---

### ✅ std::move

불필요한 객체 복사를 줄이기 위해 Move Semantics를 적용하였다.

```cpp
knownPeople.emplace_back(
    std::move(name),
    std::move(descriptor)
);
```

이를 통해 메모리 오버헤드를 줄이고 성능을 향상시켰다.

---

## 🚀 Optimization

본 프로젝트에서는 다음과 같은 최적화 기법을 적용하였다.

- Release Mode Build
- Move Semantics 기반 메모리 최적화
- 프레임 스킵 처리
- Face Alignment 기반 인식 정확도 향상
- DLL 분리 및 실행 최적화

---

## 📊 Result

### 구현 성공 항목

- 실시간 얼굴 탐지 성공
- Landmark 추출 성공
- 얼굴 정렬 성공
- 128D 특징 벡터 생성 성공
- 여러 사람 얼굴 구분 성공
- 실시간 Bounding Box 출력 성공

### Example Output

```txt
BAEG dist=0.32
SIM dist=0.28
128D face vector extracted
```

---

## ⚠ Error Handling & Debugging

프로젝트 진행 중 다음과 같은 문제를 해결하였다.

- OpenCV DLL 오류
- dlib 모델 파일 경로 오류
- Debug / Release DLL 충돌
- CMake 라이브러리 링크 오류
- 얼굴 탐지 실패 문제
- 영상 프레임 처리 속도 문제

---

## 📚 References

- https://github.com/opencv/opencv
- https://github.com/davisking/dlib
- https://github.com/davisking/dlib-models
- https://github.com/microsoft/vcpkg
- http://dlib.net/dnn_face_recognition_ex.cpp.html
