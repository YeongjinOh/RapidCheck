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
model_name = 'base-voc2007-trainval'
classes_name = ["car", "person"]

# dataset_abs_location = os.path.join('C:\\\\', 'Users', 'SoMa', 'myworkspace', 'RapidLabeling', 'app', 'static', 'datacenter')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_06-2007','VOCdevkit','VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','my_trainset', 'datacenter_mydata')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','my_trainset', 'datacenter_mydata')
test_dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtest_06-2007','VOCdevkit','VOC2007')

# ann_location = os.path.join(dataset_abs_location, 'annotations')
ann_location = os.path.join(dataset_abs_location, 'Annotations')
test_ann_location = os.path.join(test_dataset_abs_location, 'Annotations')

# imageset_location = os.path.join(dataset_abs_location, 'images')
imageset_location = os.path.join(dataset_abs_location, 'JPEGImages')
test_imageset_location = os.path.join(test_dataset_abs_location, 'JPEGImages')

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
Bottleneck layer 를 깨끗하게 초기화 시킨뒤에 voc2007 trainval 과 test set 으로 돌려보는 실험을 한다..\n\
현재 사용하고 있는  base 는 pretrain/yolo-tiny-origin-thdim-named.h5 를 이용할 것 이다. \n\
다음실험은 train loss 와 test loss History 를 효과적으로 기록하고 사용할 수 있어야 한다.".format(cell_size, 
	num_classes, 
	boxes_per_cell, class_scale, object_scale, noobject_scale, coord_scale, inp_size[0], epochs, lr, trainer, image_dim_order, norm_type,)