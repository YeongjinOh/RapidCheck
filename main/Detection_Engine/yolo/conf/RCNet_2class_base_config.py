"""
only 2class classifier RCNet model config
	person
	car
	source : RapidCheck Own DataSet
	author : SSUHan (ljs93kr@gmail.com)
"""
# Global config variables
import os

### Pretrained Source Weights Model Path
# pretrained_model = os.path.join('models', 'train', 'yolo-2class-cell14', 'from-cell14base-mydata-steps100000.h5')
# pretrained_model = os.path.join('models', 'train', 'yolo-2class-cell14-voc-dropout', 'dropout-mydata-train-steps8000.h5')
pretrained_model = os.path.join('dropbox', 'models', 'train', 'yolo-2class-cell14-voctrain', 'yolo-2class-voc2007-train-cell14-steps40000.h5')

### New Training Model Folder and Name Path
model_folder = os.path.join('models', 'train', 'rcnet-2class-base-from-voctrain')
model_name = 'mydata-reversed-trainval-dropout'

### Detected Classes Name List
classes_name = ["car", "person"]

"""
	datacenter/
		dataset_enduser_root/
			annotations/
			images/
			trainval/
			test/
			infomation.json
		video_123456/
			annotations/
			images/
		video_234567/
			annotations/
			images/
"""
datacenter_root = os.path.join('dropbox', 'dataset', 'datacenter')
dataset_enduser_root = os.path.join(datacenter_root, 'datacenter_mydata')

### Trainval Dataset Absolute Path
dataset_abs_location = os.path.join(dataset_enduser_root, 'trainval')
ann_location = os.path.join(dataset_abs_location, 'annotations')
# ann_location = os.path.join(dataset_abs_location, 'Annotations')
imageset_location = os.path.join(dataset_abs_location, 'images')
# imageset_location = os.path.join(dataset_abs_location, 'JPEGImages')

### Test Dataset Absolute Path
test_dataset_abs_location = os.path.join(dataset_enduser_root, 'test')
test_ann_location = os.path.join(test_dataset_abs_location, 'annotations')
test_imageset_location = os.path.join(test_dataset_abs_location, 'images')
### RCNet Model Config
cell_size = 14
num_classes = len(classes_name)
boxes_per_cell = 2

class_scale = 1
object_scale = 1
noobject_scale = 0.5
coord_scale = 5

inp_size = 448, 448, 3
batch_size = 32
epochs=20000

lr=0.0001
trainer='adam'

image_dim_order = 'th'
norm_type = 'scale_down'

descriptions = "2017-06-14\n\
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
dataset_abs_location : {}\n\
voctrain 으로부터 rcnet base 를 만드는데, reversed + trainval / test 를 이용하는 첫번째 모듈\n\
베이스를 만드는 과정에서 Dropout 이 효과가 있을지는 미지수이다. \n\
베이스를 Dropout 이 들어간 상태에서, mydata 도 dropout 레이어를 추가했을 시 도메인 결과에 어떠한 변화가 있는지를 확인할 필요가 있다.\n\
다음실험은 mydata 도메인에서도 dropout 레이어를 추가하여 학습하여, 스킵된 프레임에서의 안정성과 정확성을 확인한다.".format(cell_size, 
	num_classes, 
	boxes_per_cell, class_scale, object_scale, noobject_scale, coord_scale, inp_size[0], epochs, lr, trainer, 
	image_dim_order, norm_type, pretrained_model, dataset_abs_location)