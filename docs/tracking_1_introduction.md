# Tracking Introduction

RapidCheck tech documents about Tracking Engine powered by Junsu Lee (ljs93kr@gmail.com)

#### 개요

[![tracking_define.png](https://s19.postimg.org/9t0wril0j/tracking_define.png)](https://postimg.org/image/r6b76dgbj/)

*객체 추적의 정의*

  영상 속에서 특정 대상의 정보를 분석하기 위해선, 각 frame에서 검출된 대상이 같은 대상인지를 판별할 수 있어야 한다. Frame이 진행되면서 검출된 대상의 identity를 유지해 나가는 기술이 object tracking이다. 본 프로젝트는 tracking 분야에서도 MOT(Multiple Object Tracking)에 해당하는 기술을 구현하였다.

[![tracking_pipeline.png](https://s19.postimg.org/k4d9k6cpv/tracking_pipeline.png)](https://postimg.org/image/codzydp0f/)

*객체 추적 알고리즘의 Pipe Line*

  본 프로젝트에서는 Detection 기반의 Tracking algorithm을 설계하였다. 각 프레임에서 Detection을 통해 대상들의 위치를 검출하고, 프레임 간에 객체들의 Identity를 유지하기 위해서 먼저 Tracklet(짧은 구간 경로)을 구하였다. 그리고 Tracklet간의 Similarity를 기반으로 이들을 이어나가는 방식으로 Trajectory(전체 구간 경로)를 구하였다.