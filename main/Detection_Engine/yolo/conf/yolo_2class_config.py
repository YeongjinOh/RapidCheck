"""
only 2class classifier yolo model config
	person
	car
	source : VOC 2007 DataSet
	author : SSUHan (ljs93kr@gmail.com)
"""
# Global config variables
import os

pretrained_model = os.path.join('models', 'train', 'yolo-2class-cell14', 'from-cell14base-mydata-steps100000.h5')

model_folder = os.path.join('models', 'train', 'yolo-2class-cell14-mydata100000')
model_name = 'only-video_217193'
classes_name = ["car", "person"]

# dataset_abs_location = os.path.join('C:\\\\', 'Users', 'SoMa', 'myworkspace', 'RapidLabeling', 'app', 'static', 'datacenter')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_06-2007','VOCdevkit','VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','my_trainset', 'datacenter_mydata')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','my_trainset', 'datacenter_mydata')
dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','Dropbox','RapidCheck', 'dataset', 'datacenter', 'video_217193')
test_dataset_abs_location = None
ann_location = os.path.join(dataset_abs_location, 'annotations')
# ann_location = os.path.join(dataset_abs_location, 'Annotations')
test_ann_location = None
imageset_location = os.path.join(dataset_abs_location, 'images')
# imageset_location = os.path.join(dataset_abs_location, 'JPEGImages')
test_imageset_location = None

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

descriptions = "2017-06-06\n\
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
pretrained_model : {}\n\
mydata100000 에서 video_217193 데이터를 심층학습 시켜본다\n\
베이스를 만드는 과정에서 Dropout 이 효과가 있을지는 미지수이다. \n\
베이스를 Dropout 이 들어간 상태에서, mydata 도 dropout 레이어를 추가했을 시 도메인 결과에 어떠한 변화가 있는지를 확인할 필요가 있다.\n\
다음실험은 mydata 도메인에서도 dropout 레이어를 추가하여 학습하여, 스킵된 프레임에서의 안정성과 정확성을 확인한다.".format(cell_size, 
	num_classes, 
	boxes_per_cell, class_scale, object_scale, noobject_scale, coord_scale, inp_size[0], epochs, lr, trainer, image_dim_order, norm_type, pretrained_model)