
feat_path = '../data/features/neg/neg-1.feat'

from sklearn.externals import joblib

feats = joblib.load(feat_path)
print(feats)
print(type(feats))
print(feats.shape)