# Trajectory

RapidCheck tech documents about Tracking Engine powered by Junsu Lee (ljs93kr@gmail.com)

#### 개념

  Trajectory는 어떤 대상의  전체 구간 경로를 의미하며, 앞에서 구한 짧은구간경로(Tracklet)들 간의 유사도(Similarity)를 계산하여 이들을 이어붙이는 방식으로 만들어진다. 

  Tracklet들의 Similarity를 Short-term과 Long-term을 구분하여 계산한 뒤 최적의 Similarity 조합을 이어나가는 방식으로 알고리즘을 구현하였다.

#### Motion Model

[![motion_model.png](https://s19.postimg.org/vbqp613hf/motion_model.png)](https://postimg.org/image/aeuh1d5gf/)

*Trajectory Motion Model 의 개념과 수식*

  Tracklet 간의 motion cost를 구하기 위해서 forward deviation error와 backward deviation error를 모두 고려하였다. Forward deviation이란, 상대적으로 과거 시점의 tracklet T1이 앞으로 더 진행되었을 때 예측되는 위치와 그 순간의 T2의 위치 간의 차이를 의미하며, Backward deviation은 상대적으로 미래 시점의 tracklet T2의 예측되는 과거 위치와 T1의 위치 간의 차이를 의미한다.

#### Appearance Model

  Appearance model은 tracklet과 동일하게 Histogram correlation 방식을 사용하였다. 각 Tracklet의 대표 히스토그램을 추출하기 위해서, detection 결과에서 가장 높은 confidence를 갖는 노드를 기준으로 하였다.

#### Total Similarity

[![total_similarity.png](https://s19.postimg.org/6maylmpyb/total_similarity.png)](https://postimg.org/image/k37x4i09r/)

  실제 CCTV 영상에서는 tracking 대상이 오랜 기간 다른 물체에 가려지는 상황이 종종 발생한다. 이러한 경우에도 tracklet들을 연결할 수 있도록 알고리즘을 설계하였다. Long-term similarity의 경우 motion model의 정확도가 상대적으로 떨어지기 때문에 total similarity를 계산할 때 motion과 appearance model간의 비중을 동적으로 조절하여 더 정확한 값을 계산할 수 있게 하였다.

 Short-term similarity의 경우,  Motion similarity에 Jaccard similarity를 추가하여 정확도를 높일 수 있었다.