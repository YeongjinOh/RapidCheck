"""
only 2class classifier yolo model config
	person
	car
	source : VOC 2007 DataSet
	author : SSUHan (ljs93kr@gmail.com)
"""
# Global config variables
import os

model_folder = os.path.join('models', 'train', 'yolo-2class-cell14')
model_name = 'from-cell14-mydata-tracking-mydata-20000'
classes_name = ["car", "person"]

# dataset_abs_location = os.path.join('C:\\\\', 'Users', 'SoMa', 'myworkspace', 'RapidLabeling', 'app', 'static', 'datacenter')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_06-2007','VOCdevkit','VOC2007')
dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','my_trainset', 'datacenter_mydata')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','my_trainset', 'datacenter_mydata')

ann_location = os.path.join(dataset_abs_location, 'annotations')
# ann_location = os.path.join(dataset_abs_location, 'Annotations')

imageset_location = os.path.join(dataset_abs_location, 'images')
# imageset_location = os.path.join(dataset_abs_location, 'JPEGImages')

cell_size = 14
num_classes = len(classes_name)
boxes_per_cell = 2

class_scale = 1
object_scale = 1
noobject_scale = 0.5
coord_scale = 5

inp_size = 448, 448, 3
batch_size = 32
epochs=2000

lr=0.0001
trainer='adam'

image_dim_order = 'th'
norm_type = 'scale_down'

descriptions = "2017-06-05\n\
cell size : {}\n\
num_classes : {}\n\
boxes_per_cell : {}\n\
class_scale : {}\n\
object_scale : {}\n\
noobject_scale : {}\n\
coord_scale : {}\n\
inp_size : {}\n\
epochs : {}\n\
lr : {}\n\
trainer : {}\n\
image_dim_order : {}\n\
norm_type : {}\n\
cell14-mydata-tracking-mydata-20000 란 voc2007 trainval 로 초벌구이 를 한뒤에, 1만 프레임 이하의 tracking 데이터를 가지고 20000번 학습한 모델이다.\n\
현재 사용하고 있는 cell14-mydata-tracking-mydata-20000 의 이름은 yolo-2class-mydata-tracking-cell14-steps20000.h5 이다. \n\
이번에 하고자 하는 실험은 뒷부분을 추가적으로 라벨링을 하였을때, 동일한 네트워크에서 객체를 안정적으로 잡는지, validation set 에서 안정성이 올라가는지를 확인하는 것이 목표이다.\n\
다음실험은 1만 프레임 이전데이터 를 가지고, Dropout Layer 를 추가하므로서 동일 도메인에서 validation set 의 정확도와 안정성이 증대되는지를 확인할 필요가 있다.".format(cell_size, 
	num_classes, 
	boxes_per_cell, class_scale, object_scale, noobject_scale, coord_scale, inp_size[0], epochs, lr, trainer, image_dim_order, norm_type,)