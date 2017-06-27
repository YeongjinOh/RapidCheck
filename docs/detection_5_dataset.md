# Dataset 

RapidCheck tech documents about Detection Engine powered by Junsu Lee (ljs93kr@gmail.com)

  RapidCheck가 동작하는 환경의 특성상 기관과 협약을 맺는 경우가 아니고서는 CCTV 영상을 안정적으로 확보하기가 쉬운 일은 아니다. 설사 대량의 영상을 확보하더라도, 모델의 학습을 위해서 객체 위치 정답지를 만드는(Labeling) 일 역시 고난이다. 결국 양과 질의 Detection Training Dataset 이 필요했고, RapidCheck는 전 세계에 열려있는 학술용 데이터 수집에 집중했다.



#### VOC Pascal & MOT Challenge

[![voc_label.png](https://s19.postimg.org/qog1qiqqr/voc_label.png)](https://postimg.org/image/p9eh1spnj/)

*VOC Pascal (2007~2012)*

  VOC Pascal Challenge 는 객체 인식 및 분류를 목적으로 전세계 학자들을 대상으로 2007년~2012년까지 진행하였던 Object Detection 관련 저명한 대회이다. 여기로부터 2007년도 데이터로만 라벨링이 된 10,000개 이상의 고품질 이미지, 약 10600여 사람 객체 정보, 약 3000여개 이상의 자동차 이미지를 확보하였다.

[![mot_label.png](https://s19.postimg.org/twkj3kd0j/mot_label.png)](https://postimg.org/image/5g2d93c9r/)

*MOT Benchmark*

  오픈데이터는 이 뿐만이 아니다. 최근까지 진행되고 있는 MOT Benchmark는 “객체 추적” 의 관점에서 동영상에 출연하는 객체들의 “정체성”을 맞추는 한층 난이도 있는 대회이다. RapidCheck는 MOT로 부터도 양질의 데이터를 얻을 수 있었다. 2017년도 데이터로도 21개의 영상, 30만개의 객체정보를 확보 가능했다. 다만, RapidCheck에서 사용하는 도메인에 적합한 영상과 데이터로 선별하였고, MOT 라벨링 데이터의 포맷이 VOC 와 호환되지 않기 때문에, “MOT2VOC” 라는 포맷 변환 모듈을 직접 개발하였다.

#### Own Dataset

  오픈 데이터를 확보하였기 때문에, 개발과정에서 Model의 학습을 구현할 수 있었다. 하지만, 학술용으로 열려있는(Opened) 데이터와 RapidCheck의 도메인 데이터는 확연한 차이가 있었다. 바로 “객체의 크기” 와 특징이 기존과 다르다.

[![dataset_compare.png](https://s19.postimg.org/qqzxdcueb/dataset_compare.png)](https://postimg.org/image/vpnfrvy73/)

  곧바로 확인할 수 있다시피, VOC 데이터는 화면에 객체가 가득차 있는 형태가 많은 반면, 실제 CCTV 카메라 각도로서는 객체의 크기가 굉장히 작고, 사람 객체의 경우에는 비교할 수 없이 작다. 이렇듯 적용 도메인의 종류가 명확히 다른 상황이기 때문에, 기존에 학술적으로 존재하던 데이터나 모델로는 현장 정확도를 만족시키기 어렵다는 판단을 했다. 따라서 실제와 굉장히 흡사한 각도와 장소에서 직접 영상 데이터를 모으기 시작했으며, 모은 영상 데이터는 개발인원이 개발기간 동안 나눠서 손수 라벨링을 했다. 이 과정에서 영상을 넣으면 손쉽게 라벨링을 하여 VOC 데이터 포맷으로 만들어 주는 RapidLabeling 서비스를 파생 개발하였다. RapidLabeling 은 Object Detection를 연구하는 많은 서비스와 개발자들에게 편의를 제공할 수 있을 것으로 기대한다.

#### 향후 데이터 수급 전망

  RapidLabeling System으로 굉장히 빠른 속도로 원하는 도메인의 데이터를 확보할 수 있었고, 앞으로도 지속적인 정답지를 만들어 학습시킬 충분한 환경을 구축하였다. RapidLabeling 이 서비스로 확장되면, 여기서 쌓이는 집단데이터로 인해, RapidCheck Detection Engine 의 성능 업데이트는 긍정적으로 기대할 수 있다. 

  또한, MS(MicroSoft)에서 주최한 COCO Dataset 역시 아직 남아있는 양질의 데이터이다. 

#### Own Labeling System 구축

[![rapidlableing.png](https://s19.postimg.org/5vdn23y77/rapidlableing.png)](https://postimg.org/image/imrt8m7z3/)

*RapidLabeling 예시화면*

  본 프로젝트를 진행하면서, 도메인에 맞는 데이터를 구하기 위하여 가상의 환경을 만들고 직접 영상을 촬영하였다. 이 데이터들을 학습용으로 사용하기 위해 라벨링이 필요하였으며, 라벨링을 편하게 하고 자동으로 학습할 수 있는 포맷으로 xml을 내려주는 라벨링 시스템을 구축하였다.