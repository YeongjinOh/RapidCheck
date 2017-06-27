# Tracklet

RapidCheck tech documents about Tracking Engine powered by Junsu Lee (ljs93kr@gmail.com)

#### Tracklet 을 구하는 과정

[![tracking_1.png](https://s19.postimg.org/e4pigirxf/tracking_1.png)](https://postimg.org/image/p4aps4icf/)

*(a) detection on a image*

 짧은 구간(k개의 Frame)에 대해 Detection을 수행한다. 다양한 이유로 인해, 모든 Frame에서 객체가 정확하게 검출되기는 힘들다. 그럼에도 불구하고, 객체의 위치를 계속 추적해나가는 것이 목적이다.

[![tracking_2.png](https://s19.postimg.org/pic1rq2g3/tracking_2.png)](https://postimg.org/image/60hebs5i7/)

*(b) 검출된 객체들 간의 유사도를 계산한다 (Appearance model + Motion model)*

  검출된 각각의 객체들에 대해서 Apperance model value 와 Motion model valule 를 계산한다. Apperence model 과 Motion model 의 의미는 뒷 섹션에서 자세히 설명한다.

[![tracking_3.png](https://s19.postimg.org/e7dzh3kyr/tracking_3.png)](https://postimg.org/image/4mucu7vmn/)

*(c) 최적의 유사 조합을 검출한다*

  계산된 value들의 값을 적절하게 조합하면 최종 유사 조합을 검출할 수 있다. 이 과정에서 Detection 이 되지 않은 Frame으로 인해 자연스럽게 정답이 아닌 객체(Outlier) 가 생기게 된다.  Outlier 는 생길 수 밖에 없는 어쩔수없는 오류인데, 이를 보완하는 것이 **RapidCheck Tracking Engine 의 핵심**이다.

[![tracking_4.png](https://s19.postimg.org/srv2bxfxf/tracking_4.png)](https://postimg.org/image/5qeh66g9r/)

*(d) Outlier 를 제거하고, Dummy Node 를 추가한다*

  모든 프레임에서 대상을 정확하게 찾아낼 수 없기 때문에 Tracking은 Detection의 빈 자리를 채우는 역할을 해야한다. 예를 들어, (b) 를 보면 빨간 옷을 입은 여성이 다른 물체에 가려져 3,4 번째 프레임에서는 검출되지 않았다. 따라서 최적의 유사 조합에 Outlier가 함께 나타난다. [c] 이러한 문제를 해결하기 위해서 본 프로젝트에서는 Linear queue를 가정한 position 기반의 모델을 사용하여 Outlier를 제거하고 Dummy Node를 추가하였다. 그 결과 [e]과 같은 결과를 얻을 수 있다.

[![tracking_5.png](https://s19.postimg.org/re3fgmgo3/tracking_5.png)](https://postimg.org/image/tvf6nw0kf/)

*(e) Tracklet의 예시*

#####  Model Model on Tracklet

[![motion_model_trackelt.png](https://s19.postimg.org/xhl00j6xv/motion_model_trackelt.png)](https://postimg.org/image/qed4kx1i7/)

*Tracklet 에서의 Motion cost model*

  짧은 구간에서 객체의 움직임은 선형이라는 성격을 이용하면 Motion model의 Cost를 위와 같이 계산할 수 있다. 위 수식에서   ![img](file:///C:\Users\Soma2\AppData\Local\Temp\DRW0000269c2a45.gif)  에 해당하는 부분은 i번째 프레임에서의 객체의 위치를 이전 프레임들의 위치를 토대로 예측한 값이며, 이를 실제 I번째 위치와의 오차와 비교하여 Cost를 계산한다.

##### Appearance Model on Tracklet

[![appearance_model.png](https://s19.postimg.org/9f465nqar/appearance_model.png)](https://postimg.org/image/s7g198mov/)

*Tracklet에서의 Appearance cost model*

  Appearance model로는 색상 히스토그램을 사용하였으며, 그림자와 같은 외부 요인의 영향을 줄이기 위해 HSV를 기준으로 하였다. 컴퓨터 비전에서 널리 사용되는 Histogram Correlation방법으로 Cost를 계산하였다.

#### Trajectory

  Trjectory는 어떤 대상의  전체 구간 경로를 의미하며, 앞에서 구한 Tracklet들 간의 Similarity를 계산하여 이들을 이어붙이는 방식으로 만들어진다. 

  Tracklet들의 Similarity를 Short-term과 Long-term을 구분하여 계산한 뒤 최적의 Similarity 조합을 이어나가는 방식으로 알고리즘을 구현하였다. 

##### Motion Model on Trajectory

[![motion_model.png](https://s19.postimg.org/vbqp613hf/motion_model.png)](https://postimg.org/image/aeuh1d5gf/)

  Tracklet 간의 motion cost를 구하기 위해서 forward deviation error와 backward deviation error를 모두 고려하였다. Forward deviation이란, 상대적으로 과거 시점의 tracklet T1이 앞으로 더 진행되었을 때 예측되는 위치와 그 순간의 T2의 위치 간의 차이를 의미하며, Backward deviation은 상대적으로 미래 시점의 tracklet T2의 예측되는 과거 위치와 T1의 위치 간의 차이를 의미한다.

##### Appearance Model on Trajectory

  Appearance model은 tracklet과 동일하게 Histogram correlation 방식을 사용하였다. 각 Tracklet의 대표 히스토그램을 추출하기 위해서, detection 결과에서 가장 높은 confidence를 갖는 노드를 기준으로 하였다.

##### Total Similarity

[![total_similarity.png](https://s19.postimg.org/6maylmpyb/total_similarity.png)](https://postimg.org/image/k37x4i09r/)

  실제 CCTV 영상에서는 tracking 대상이 오랜 기간 다른 물체에 가려지는 상황이 종종 발생한다. 이러한 경우에도 tracklet들을 연결할 수 있도록 알고리즘을 설계하였다. Long-term similarity의 경우 motion model의 정확도가 상대적으로 떨어지기 때문에 total similarity를 계산할 때 motion과 appearance model간의 비중을 동적으로 조절하여 더 정확한 값을 계산할 수 있게 하였다.

  Short-term similarity의 경우,  Motion similarity에 Jaccard similarity를 추가하여 정확도를 높일 수 있었다.