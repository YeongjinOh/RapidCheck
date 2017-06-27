# Data Preprocessing 

RapidCheck tech documents about Detection Engine powered by Junsu Lee (ljs93kr@gmail.com)

#### Preprocessing

  다음은 RCNet 이 학습을 하기 위해서, 라벨링한 이미지를 어떤 방법으로 전처리(Preprocessing)를 하는지 예시사진으로 설명한다.

[![rcnet-preprocessing3.png](https://s19.postimg.org/c652davab/rcnet-preprocessing3.png)](https://postimg.org/image/vnzpt8s7z/)

*Step1. Grid Cell 만큼, 이미지의 위치를 상대적으로 분할한다*

  먼저 위와 같이 이미지가 들어왔을 때, 객체의 중앙정보(Central Infomation)을 포함하는 Grid Cell 로 논리분할한다. 

[![rcnet-preprocessing2.png](https://s19.postimg.org/66hb9nahv/rcnet-preprocessing2.png)](https://postimg.org/image/6j8pftsrj/)

*Step2. 중앙점 정보를 담을 Grid Cell 의 인덱스*

  분할된 Grid Cell 은 그 고유 인덱스(Index)으로 대표할 수 있고, 이는 이미지내 중앙점의 위치를 특정할 수 있다.

[![rcnet-preprocessing1.png](https://s19.postimg.org/a44kz1xb7/rcnet-preprocessing1.png)](https://postimg.org/image/91uegiehr/)

*Step3. cx,cy,w,h 상대값 처리*

  이로써, 객체가 위치하는 Cell Index 안에 포함되는 객체의 center_x, center_y를 구해낼 수 있고, 이는 Cell 내부에서 상대적인 위치로 표현된다. width, height는 이미지 전체 크기를 기준으로 상대값으로 표현된다. 

  위와 같은 방법으로 각 학습이미지는 객체마다 중앙점정보와 상대적 크기 정보를 흭득하게 되고, 학습의 결과물과 동일한 차원(Dimension)으로 변환된다.



#### Data Argumentation

   학습 과정 중 데이터의 간단한 조작만으로도, 적은 데이터로부터 상대적으로 풍부한 데이터를 얻을 수 있다. RapidCheck Detection Engine에서 선택한 Data Argumentation 방법은 “Reversed Image”다.

