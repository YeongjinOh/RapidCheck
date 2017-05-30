"""
only 2class classifier yolo model config
	person
	car
	source : VOC 2007 DataSet
	author : SSUHan (ljs93kr@gmail.com)
"""
# Global config variables
import os


model_name = 'yolo-2class-mydata-with-voc2007-train-cell14'
classes_name = ["car", "person"]

# dataset_abs_location = os.path.join('C:\\\\', 'Users', 'SoMa', 'myworkspace', 'RapidLabeling', 'app', 'static', 'datacenter')
# dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_06-2007','VOCdevkit','VOC2007')

dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','my_trainset', 'voc2007_train_mydata_tracking', 'datacenter')

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
epochs=500

lr=0.0001
trainer='adam'

image_dim_order = 'th'
norm_type = 'scale_down'