import os
import torch
import torchvision.transforms as transforms
from torchvision.models import vit_b_16
from PIL import Image
import pandas as pd

# 设置设备
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 加载预训练的 ViT 模型
model = vit_b_16(pretrained=True).to(device)
model.eval()

# 定义图像预处理
preprocess = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

def extract_features(image_path):
    """提取图像特征"""
    image = Image.open(image_path).convert('RGB')
    image = preprocess(image).unsqueeze(0).to(device)
    with torch.no_grad():
        features = model(image)
    return features.cpu().numpy().flatten()

def process_dataset(image_dir, prefix):
    """处理数据集并提取特征"""
    data = []
    image_files = sorted([f for f in os.listdir(image_dir) if f.startswith(prefix) and f.endswith(".png")], 
                         key=lambda x: int(x.split('_')[1].split('.')[0]))

    for image_name in image_files:
        image_id = int(image_name.split('_')[1].split('.')[0])
        image_path = os.path.join(image_dir, image_name)
        features = extract_features(image_path)
        data.append([image_id] + features.tolist())
        if image_id%1000 == 0:
            print('name ', image_name)

    return data


# 设置图片目录
train_dir = 'C:\\Users\\daidai\\Desktop\\课件\\大二下\\机器学习\\作业\\大大大作业\\train'
test_dir = 'C:\\Users\\daidai\\Desktop\\课件\\大二下\\机器学习\\作业\\大大大作业\\test'

# 提取训练集特征
train_data = process_dataset(train_dir, 'train')
train_df = pd.DataFrame(train_data)
train_df.to_csv('train_features1.csv', index=False, header=False)

# 提取测试集特征
test_data = process_dataset(test_dir, 'test')
test_df = pd.DataFrame(test_data)
test_df.to_csv('test_features1.csv', index=False, header=False)

print("特征提取完成并存储到 CSV 文件中。")
