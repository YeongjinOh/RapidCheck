# Introduction

RapidCheck tech documents about Detection Engine powered by Junsu Lee (ljs93kr@gmail.com)

####  개요

  RapidCheck의 객체검출 알고리즘을 자세하게 풀어쓴다면 “Object Detection on a Single Frame in Video” 라고 할 수 있다. 즉, 영상에서 객체를 검출함에 있어, 일반적으로 사용하는 여러 프레임간의 차이를 이용해 움직이는 객체만 검출할 수 있는 알고리즘과 근본적인 차이점과 난이도를 가지고 있다.

  여러 프레임 간의 차이를 이용해서 객체를 검출하는 방법은 기본적으로 고정된 영상 분석을 전제로 한다. 한 영상에서 인접한 두 프레임 이미지 간의 차이(Difference)를 구해, 배경을 제거하고 움직이는 물체의 위치를 찾아내는 원리이다. 이 과정에서 더 깨끗한 영역을 검출하기 위해 노이즈 제거, 근접한 프레임들의 평균값으로 정규화하는 등 컴퓨터비전(Computer Vision) 적인 전처리를 동반한다. 언뜻보면 간단한 원리이지만, 실제로 현장에서 적용하기에는 심각한 한계점이 있다.  이는 다음과 같다.



```
- 영상 속에서 멈춰있는 객체는 검출하지 못함
- 카메라의 화각이 고정되어 있어야 함 (배경이 움직이는 영상에서는 사용할 수 없음)
- 비전(Vision) 전처리를 위해서 엄청난 CPU Computing Resources 를 요구함
- 객체의 위치를 검출할 뿐, 객체가 무엇인지는 검출할 수 없음 (이 단계를 위해서는 추가적인 단계가 필요함)
```

 

  따라서, RapidCheck Detection 알고리즘은 **Single Frame in Video Detection 기술구현**에 집중하였다. 다양한 환경에서 보다 정확하고, 많은 객체를 찾아내기 위해서 수많은 시도와 실험을 시도하였고, 결론적으로 RapidCheck 의 Detection Engine 은 Deep Learning 중 Supervised Learning(지도학습) 으로 대량의 이미지 데이터들을 학습함으로써 설계하였다. 이를 RCNet 라고 명명하였으며, 구조는 다음과 같다. 

[![new_rcnet_arch2.png](https://s19.postimg.org/l8oy6l5nn/new_rcnet_arch2.png)](https://postimg.org/image/pul2exr6n/)

  

#### Computer Vision vs Deep Learning

  Object Detection on Single Frame 의 기술이 Deep Learning 기법을 도입해야만 해결할 수 있는 문제는 아니다. Computer Vision 에서도 오랫동안 이를 위한 기술연구가 존재한다. 그 중 보행자 추출에 성능을 보이는 HoG 기법을 먼저 적용해보았다.

  HoG (Histogram of Gradients) 는 이미지의 한 픽셀에 대해서 인접한 픽셀데이터들의 기울기벡터(Gradient Vectors)를 이용해. 일정 영역안의 특징을 구해내고, 그 특징을 Histogram 으로 계산한다. 이는 Histogram 데이터가 일관된 벡터상의 데이터로 표현되기 때문에, 목표클래스(Target) 비목표 클래스(non-Target) 로 구분하여 저장할 수 있다. 이후. 벡터를 이용한 데이터 분류기인 SVM(Support Vector Machine) 으로 클래스를 구분해내는 원리이다. Histogram of gradients를 생성하는 과정에서 정규화과정을 거치기 때문에, 픽셀의 절대값이나 채도의 영향을 받지 않아, 패턴매칭 알고리즘(Pattern Matching) 중에서는 우수한 성능을 얻어내긴 하지만, RapidCheck 가 HoG 알고리즘을 직접 구현하고, 실전에 적용해본 결과 이 역시 무시할 수 없는 한계점을 드러냈다.

[![hog_limit.png](https://s19.postimg.org/qlidygixv/hog_limit.png)](https://postimg.org/image/lzm9q3xen/)

  이와 더불어, HoG 는 원리상 고정된 크기의 필터로 Sliding Window 방식으로 옮겨가며 구해내기 때문에, 영상속에서 출연하는 객체의 크기가 필터의 사이즈와 정확하게 일치해야만 의미가 있다. 비록, CCTV의 높이와 각도가 공공기관의 경우에는 규정이 있다고 하더라도 원근에 따른 객체의 크기 변화를 감당하기에는 HoG 기법이 근본적인 한계를 가지고 있다. 물론 다양한 크기를 찾아내기 위해서 이미지 피라미드 (Image Pyramid) 의 방법으로 찾아낼 수 있지만, 이는 엄청난 CPU Computing Power 를 야기하며, 시간복잡도를 심각하게 높이는 병목이 된다. 

  위와 같은 문제를 해결하기 위해서 RapidCheck 는 Deep Learning 기법을 적용하였다. Deep Learning을 이용해서 얻을 수 있는 기대효과는 다음과 같다.

```
- 다양한 각도, 다양한 크기의 객체를 추가 Computing 자원소모 없이 검출할 수 있다.
- 데이터 전처리 로직은 CPU에서 담당하고, Detecting 과정은 GPU에서 담당하는 등 GPU 가속의 효과를 얻을 수 있다.
- 학습시키면 학습시킬수록 안정적이고 일반적인(Gereral) 결과물을 얻을 수 있다. 
```

  RapidCheck Detection Engine은 Google이 관리하는 ML Framework 인 **TensorFlow**으로 구성되었으며, 다양한 모델링과 실험의 용이를 위해서 Wrapper Class 인 **Keras**를 함께 응용하였다.