# Deep Algorithm Comparsion

RapidCheck tech documents about Detection Engine powered by Junsu Lee (ljs93kr@gmail.com)

#### R-CNN vs YOLO

  Deep Learning 기반 Object Detection 기법에는 크게 두 가지 부류로 나뉜다. RapidCheck는 이 두 가지 알고리즘의 특징을 분석하고, 프로젝트 목적에 적합한 방법론을 채택하였다. 

  먼저, R-CNN 계열이다. “Region Based CNN” 의 약자로 기본적으로 지도학습(Supervised Learning) 중 분류(Classification) 모델을 기초로 한다. 

[![rcnn.png](https://s19.postimg.org/uj5nnv5r7/rcnn.png)](https://postimg.org/image/4nlx4o3xb/)

*R-CNN 계열의 Object Detection 원리*

  R-CNN은 “예상범위추출(Region Proposals Network)”을 통해서 계산할 RoI를 뽑아내는 단계 다음에, 해당 위치가 어떤 클래스인지를 구분하는 “분류 단계”로 객체를 구해내는 원리이다. 즉, R-CNN은 두 단계의 계산을 진행하고, Fast R-CNN 계열이 아닌 R-CNN 의 경우, 시간복잡도 뿐만 아니라 공간복잡도까지 급격하게 증가한다. 공개되어 있는 객관적인 Evaluation을 확인해보면, 정확도는 보다 높게 측정되었더라도, 분석 시간적 측면에서는 단점을 가지고 있다. RapidCheck의 사용자 시나리오를 고려해보았을 때, “원하는 대상을 빠르게” 찾는 부분에서 충분한 이슈가 생길 것으로 판단하였다. 게다가, RapidCheck가 동작하는 환경인 Windows에서 R-CNN의 근간 프레임워크인 Caffe의 복잡한 종속성(Dependency) 역시 실험결과 배제의 대상이 되었다.

  YOLO 계열은 “You Only Look Once” 의 약자로, 말그대로 한번의 계산으로 “위치와 종류”를 얻어내는 알고리즘이다. Deep Learning 방법 중 Image 분야에서 회귀(Regression) 모델로 접근했다는 특징이 있다. 

[![yolo-look.png](https://s19.postimg.org/ytkbjgaub/yolo-look.png)](https://postimg.org/image/p90owkli7/)

*YOLO 의 전처리 원리*

  이미지를 전체 Grid Cell 로 분할하여, 객체의 존재여부를 예측하고, 동시에 객체의 종류(Class)를 예측한다. 즉, 한번의 계산(One-Shot Detecting) 으로 위치와 종류를 구분할 수 있다. 이를 위해서 손실함수(Loss function)을 구성하는데 가공할만한 난이도가 존재하지만, RapidCheck는 구현을 시도, 성공하였으며, 기존 YOLO network 의 성능상의 이슈를 향상시킨 RCNet을 마침내 구현할 수 있었다. 현재 RapidCheck Detection Engine 은 RCNet 으로 이루어져 있다.