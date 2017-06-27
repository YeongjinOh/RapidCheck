# Model Structure & Learning Mechanism

RapidCheck tech documents about Detection Engine powered by Junsu Lee (ljs93kr@gmail.com)



RCNet 은 논리적으로 다음 두 단계의 학습을 거친다.

#### Feature Extraction Layer 구조 및 학습

  RCNet 의 구조도를 보면 확인할 수 있듯이 Feature Extraction Layer에서 이미지 속의 객체의 특징정보를 추출해내는 역할을 수행하며, 실질적으로 Detection Layer에서 특징을 연결하여 결과물을 도출하는 역할을 수행한다. 즉, 두 Layer 는 논리적으로 독립이 되어있다. 따라서 두 Layer는 학습도 다른 방법으로 진행되어야 할 필요가 있었기 때문에 전이학습(Transfer Learning) 기법을 도입했다.

  **전이 학습**이란, 개인의 한정된 컴퓨팅 리소스에서 복잡하고 무거운 모든 Model들을 학습시키기에는 힘들기 때문에, 이미 학습된 다른 모델의 Weights 값을 가져와 학습시키는 방법을 의미한다.

  [![transferlearning.png](https://s19.postimg.org/lah9iz10j/transferlearning.png)](https://postimg.org/image/6eiqbdplr/)

*전이 학습(Transfer Learning) 의 모형도*

  위 모형도에서 볼 수 있듯이, 모델A에서 학습한 Knowledge를 모델 B에서 이어받아 사용하는 기법이다. 전이 학습을 적용하면 다음과 같은 효과를 얻을 수 있다.

```
- 이전에 학습한 복잡한 모델의 지식을 그대로 이어받아 학습시간 단축이 가능하다.
- 논리적으로 구분되어야 할 Layer 간 다른 학습을 진행할 수 있다.
```

  특징 추출 계층에서는 자동차와 사람의 특징을 이루는 점, 선, 면, 질감, 색감 등의 특징을 명확하게 뽑아내도록 학습될 필요가 있다. 이를 위해서 1.4TB 가량의 규모인 “Imagenet Challenge” 데이터로 특징 추출 계층의 선행학습을 진행한다. 누군가 잘학습해둔 모델의 Weights를 가져와 실험도 가능하다. 이렇게 충분히 학습한 특징추출계층의 아랫부분(Bottleneck)을 떼어내고, 비로소 RCNet 의 Detection Layer를 삽입한다.

#### Detection Layer 구조 및 학습

  Detection Layer 는 앞서 특징추출계층의 선행학습이 진행된 뒤, 그 Weights를 이어받아 후행학습을 진행한다. 이 과정에서 제대된 결과를 얻기 위해서는, Detection Layer Weights 는 학습가능하지만, 특징추출계층의 Weights는 학습되지 않게 만드는 “얼리기(Freezing)” 작업이 필요하다. 이것이 전이학습의 핵심이다. Tensorflow 에서는 노드를 구성할 때, trainable 이라는 매개변수로 학습여부를 선택할 수 있지만, Keras 와 Tensorflow를 섞어 응용하는 과정에서 이슈가 발생했었다. 이를 어떻게 해결했는지에 대해서는 “구현 및 이슈해결 파트”에서 다뤄져있으므로 참고할 수 있다. FC(Fully Connected) Layer 와 Dropout Layer 로 구성되어 있는 Detection Layer 는 최종적으로 다음과 같은 일차원적인(Linear) 결과(Output)를 도출한다.

[![loss_detection_layer.png](https://s19.postimg.org/50r3g2qcj/loss_detection_layer.png)](https://postimg.org/image/gd3oxuz1b/)

  크게 3가지의 논리적인 변수의 결합으로 구성하였다. Grid Cell 당 Object의 존재여부를 예측하는 Confidence Area, 객체의 위치를 예측하는 Box Coordinates Area, 객체의 종류를 예측하는 Probability Area로 이루어져 있다. 이는 다음과 같은 수식으로 계산되어 학습된다.

  RCNet 의 Loss Function 의 기초는 MSE(Mean Squared Error)를 기반하는 회귀(Regression) 예측이다. 크게보면 객체가 존재하는 Cell 과 존재하지 않는 Cell 에서의 Loss 계산이 다르게 적용되며, 기본적으로 Confidence 가 1인 Cell에서 비로소 중앙값의 상대적 위치와 객체의 가로길이와 세로길이의 상대값, 종류(Class)값을 각각 학습한다. 굉장히 기본적인 “MSE 손실 알고리즘”을 사용하더라도, 사용의 방법에 따라서 강력한 예측이 가능함을 알 수 있다. Learning Rate를 조금만 높혀도 Gradient Exploding 현상이 발생했기 때문에, 학습에 사용했던 lr 은 0.0001이고, trainer는 “Adam”을 사용했다.