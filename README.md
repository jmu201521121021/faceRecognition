# Face Recognition system base Arcface

By Jinshan Chen

# **Face Recognition System demo**

![demo](https://github.com/jmu201521121021/faceRecognition/blob/master/image/demo.png)

# Introduction

​		Main contents include: design of darknet-53-based face detection algorithm; Seetaface2 is adopted to detect key points, and affine transformation matrix is used for  face alignment. The Resnet trunk network is used to extract facial features,and the Loss function ArcFace Loss is used to train the face recognition model. Finally, the experimental results were analyzed. The mAP of the FACE detection model on the verification set of WIDER FACE was 0.841(Easy), 0.833(Medium) and 0.693(Hard), and the verification accuracy of the FACE recognition model on LFW was up to 99.4%. In addition, on the basis of the trained model, a face attendance system is designed and developed, base Qt and OpenCV, which is suitable for university  campus associations and small-scale meetings .

# FaceDetection

​		Facedetection  base yolov3.

# FaceAlignment

​		FaceAlignmet base seetaface2.

# FaceRecognition

​		FaceRecognition base ArcFace.

# Demo

# Contact

```
[Jinshan Chen](jinshanchen[at]jmu.edu.cn)
```

