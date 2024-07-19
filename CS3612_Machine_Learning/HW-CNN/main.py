import numpy as np
import struct
from array import array
import torch
import torch.nn as nn
import torch.nn.functional as F
import matplotlib.pyplot as plt


# Set hyperparameters
batch_size = 16
num_epochs = 100
learning_rate = 1e-4

# Define file paths
train_images_filepath = 'data/train-images-idx3-ubyte/train-images-idx3-ubyte'
train_labels_filepath = 'data/train-labels-idx1-ubyte/train-labels-idx1-ubyte'
test_images_filepath = 'data/t10k-images-idx3-ubyte/t10k-images-idx3-ubyte'
test_labels_filepath = 'data/t10k-labels-idx1-ubyte/t10k-labels-idx1-ubyte'


# Define a CNN model
class myCNN(nn.Module):
    def __init__(self):
        super(myCNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 16, kernel_size=5, padding=0, stride=1)
        self.conv2 = nn.Conv2d(16, 32, kernel_size=5, padding=0, stride=1)
        self.fc1 = nn.Conv2d(32, 100, kernel_size=4, padding=0, stride=1)  # Use Conv2d as fully connected layer
        self.fc2 = nn.Linear(100, 100)
        self.fc3 = nn.Linear(100, 10)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)), 2, stride=2)
        x = F.max_pool2d(F.relu(self.conv2(x)), 2, stride=2)
        x = F.relu(self.fc1(x))
        x = x.view(-1, 100)
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return F.softmax(x, dim=1)
    
# Data loading functions
def read_images_and_labels(images_filepath, labels_filepath, training=True):        
    labels, images = [], []
    # Check file paths
    with open(labels_filepath, 'rb') as file:
        magic, size = struct.unpack(">II", file.read(8))
        if magic != 2049:
            raise ValueError('Magic number mismatch, expected 2049, got {}'.format(magic))
        labels = array("B", file.read())        
    
    with open(images_filepath, 'rb') as file:
        magic, size, rows, cols = struct.unpack(">IIII", file.read(16))
        if magic != 2051:
            raise ValueError('Magic number mismatch, expected 2051, got {}'.format(magic))
        image_data = array("B", file.read())       
    
    # Convert images to numpy array
    for i in range(size):
        images.append([0] * rows * cols)
    for i in range(size):
        img = np.array(image_data[i * rows * cols:(i + 1) * rows * cols])
        img = img.reshape(28, 28)
        images[i][:] = img

    # Randomly select 300 samples for each digit for training
    if training:
        indices = []
        for i in range(10):
            indices += [j for j in range(len(labels)) if labels[j] == i][:300]
        labels = [labels[i] for i in indices]
        images = [images[i] for i in indices]

    # Rescale pixel values to [-0.5, +0.5]
    for img in images:
        img[:] = [pixel / 255.0 - 0.5 for pixel in img]
    
    return images, labels

def load_data(train_images_filepath, train_labels_filepath, test_images_filepath, test_labels_filepath):
    train_images, train_labels = read_images_and_labels(train_images_filepath, train_labels_filepath, True)
    test_images, test_labels = read_images_and_labels(test_images_filepath, test_labels_filepath, False)
    return train_images, train_labels, test_images, test_labels


# Load data
train_images, train_labels, test_images, test_labels = load_data(train_images_filepath, train_labels_filepath, test_images_filepath, test_labels_filepath)

# Convert data to torch tensor
train_images = torch.tensor(train_images).unsqueeze(1).float()
train_labels = torch.tensor(train_labels).long()
test_images = torch.tensor(test_images).unsqueeze(1).float()
test_labels = torch.tensor(test_labels).long()

use_cuda = torch.cuda.is_available()

# Train the model and draw the loss curve
model = myCNN().cuda() if use_cuda else myCNN()
loss_fn = nn.CrossEntropyLoss().cuda() if use_cuda else nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
train_losses = []
for epoch in range(num_epochs):
    for i in range(0, len(train_images), batch_size):
        if use_cuda:
            batch_images = train_images[i:i + batch_size].cuda()
            batch_labels = train_labels[i:i + batch_size].cuda()
        else:
            batch_images = train_images[i:i + batch_size]
            batch_labels = train_labels[i:i + batch_size]
        optimizer.zero_grad()
        loss = loss_fn(model(batch_images), batch_labels)
        loss.backward()
        optimizer.step()
        train_losses.append(loss.item())
    print('Epoch [{}/{}], Loss: {:.4f}'.format(epoch+1, num_epochs, loss.item()))

plt.plot(train_losses)
plt.show()


# Test the model and draw the loss curve
correct = 0
test_losses = []
with torch.no_grad():
    for i in range(0, len(test_images), batch_size):
        if use_cuda:
            batch_images = test_images[i:i + batch_size].cuda()
            batch_labels = test_labels[i:i + batch_size].cuda()
        else:
            batch_images = test_images[i:i + batch_size]
            batch_labels = test_labels[i:i + batch_size]
        outputs = model(batch_images)
        _, predicted = torch.max(outputs.data, 1)
        correct += (predicted == batch_labels).sum().item()
        test_loss = loss_fn(outputs, batch_labels)
        test_losses.append(test_loss.item())
    print('Test Loss: {:.4f}'.format(sum(test_losses) / len(test_losses))
          + ', Accuracy: {:.2f}%'.format(correct / len(test_images) * 100))

plt.plot(test_losses)
plt.show()

