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
model_name = 'from-cell14base-mydata'
classes_name = ["car", "person"]

# dataset_abs_location = os.path.join('C:\\\\', 'Users', 'SoMa', 'myworkspace', 'RapidLabeling', 'app', 'static', 'datacenter')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_06-2007','VOCdevkit','VOC2007')

dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','my_trainset', 'datacenter_mydata')

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
cell14 base 란 yolo tiny network 에서 cell_size = 14 증폭시킨 네트워크 에서 voc 2007 trainval 데이터로 초벌구이를 한 베이스를 말한다.\n\
현재 사용하고 있는 cell14 base weights 의 이름은 yolo-2class-voc2007-train-cell14-steps40000.h5 이다. \n\
이번에 하고자 하는 실험은 cell14 base 로 부터 라벨링한 여러 도메인의 데이터들을 모두 넣어서 학습시켰을때, 어느 효과를 갖는지를 확인하고자 한다.".format(cell_size, 
	num_classes, 
	boxes_per_cell, class_scale, object_scale, noobject_scale, coord_scale, inp_size[0], epochs, lr, trainer, image_dim_order, norm_type,)